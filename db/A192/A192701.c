/*
 * A192701  Number of 3X2 integer matrices with each row summing to zero, row
 *          elements in nondecreasing order, rows in lexicographically
 *          nondecreasing order, and the sum of squares of the elements <=
 *          2*n^2 (number of collections of 3 zero-sum 2-vectors with total
 *          modulus squared not more than 2*n^2, ignoring...
 * https://oeis.org/A192701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples y1 ≥ y2 ≥ y3 ≥ 0 with y1² + y2² + y3² ≤
 *            n². This is equivalent to the number of 3×2 integer matrices
 *            with zero‑sum rows, nondecreasing rows and elements, whose
 *            squared‑norm does not exceed 2 n²
 * Range:     u8 n <= 12, u16 n <= 89, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root (floor) of 0 ≤ x ≤ max_val².
 * The binary search terminates in at most log₂(max_val)+1
 * iterations and uses 128‑bit arithmetic to avoid overflow. */
static inline idx_t isqrt_idx(idx_t x, idx_t max_val) {
    idx_t low = 0, high = max_val;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq <= (__int128)x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;            /* high is floor(sqrt(x)) */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t count = 0;
    idx_t nsq = n * n;                 /* n² fits in idx_t for n ≤ 10⁴ */

    for (idx_t y1 = 0; y1 <= n; ++y1) {
        idx_t y1sq = y1 * y1;
        if (y1sq > nsq) break;

        idx_t rem_total = nsq - y1sq;            /* remaining squared sum */
        idx_t s = isqrt_idx(rem_total, n);       /* floor(sqrt(rem_total)) */

        for (idx_t y2 = 0; y2 <= y1; ++y2) {
            idx_t y2sq = y2 * y2;
            if (y2sq > rem_total) break;

            idx_t rem = rem_total - y2sq;
            while (s > 0 && (__int128)s * s > (__int128)rem) --s;
            idx_t max_y3 = (s < y2) ? s : y2;     /* min(s, y2) */
            count += (seq_t)(max_y3 + 1);
        }
    }
    return count;
}
