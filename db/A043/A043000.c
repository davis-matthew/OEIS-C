/*
 * A043000  Number of digits in all base-b representations of n, for 2 <= b <=
 *          n.
 * https://oeis.org/A043000
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n-1) + Σ_{i=1..⌊log₂ n⌋} (⌊n^{1/i}⌋ - 1) for n ≥ 2 The
 *            sum counts the extra digits contributed by the perfect‑power
 *            bases of n
 * Range:     u8 n <= 120, u16 n <= 9321, u32 n <= 9321, u64 n <= 9321,
 *            u128 n <= 9321, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor of log₂ for a 64‑bit unsigned value. */
static inline idx_t floor_log2_idx(idx_t v)
{
    idx_t r = 0;
    while (v > 1) {        /* v ≥ 2 when this is called              */
        v >>= 1;
        ++r;
    }
    return r;
}

/* Integer floor of the k‑th root of n (n ≥ 1, k ≥ 1). */
static inline idx_t nth_root_floor(idx_t n, idx_t k)
{
    if (k == 1)
        return n;
    idx_t low  = 1;
    idx_t high = n;
    idx_t best = 1;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t pow = 1;
        int ok   = 1;

        for (idx_t i = 0; i < k; ++i) {
            if (pow > n / mid) {  /* would overflow or exceed n   */
                ok = 0;
                break;
            }
            pow *= mid;
        }

        if (ok && pow <= n) {
            best = mid;
            low  = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* undefined below the offset     */

    seq_t result = (seq_t)n - 1; /* every base contributes one digit */

    idx_t log2n = floor_log2_idx(n);
    for (idx_t i = 1; i <= log2n; ++i) {
        idx_t root = nth_root_floor(n, i);
        result += (seq_t)(root - 1);
    }
    return result;
}
