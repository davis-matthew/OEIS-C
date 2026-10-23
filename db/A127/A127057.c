/*
 * A127057  Triangle T(n,k), partial row sums of the n-th row of A127013 read
 *          right to left.
 * https://oeis.org/A127057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{d | r , d ≥ k} r/d, where r is the row index such
 *            that r(r+1)/2 ≥ n and k = n – r(r–1)/2
 * Range:     u8 n <= 5778, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Code uses only integer operations and standard headers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Internal helper: triangular number of x: x*(x+1)/2 */
static inline idx_t triangular(idx_t x) {
    return (idx_t)((__int128)x * (x + 1u) / 2u);
}

seq_t a(idx_t n) {
    /* Find row r so that triangular(r) >= n */
    idx_t low = 1, high = 1;
    while (triangular(high) < n) {
        high <<= 1;            /* double until big enough */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;               /* r ≥ 1 */

    /* Compute column index k in this row */
    idx_t k = n - triangular(r - 1);   /* n - (r-1)*r/2 */

    /* Sum over divisors d of r with d ≥ k: add r/d */
    seq_t sum = 0;
    for (idx_t d = k; d <= r; ++d) {
        if (r % d == 0) {
            idx_t part = r / d;            /* r/d fits in idx_t */
            sum += (seq_t)part;            /* cast to seq_t */
        }
    }
    return sum;
}
