/*
 * A092525  To binary representation of n, append as many ones as there are
 *          trailing zeros.
 * https://oeis.org/A092525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n << v2(n)) + (2^{v2(n)} – 1) (v2(n) = number of
 *            trailing zeros in n)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count trailing zeros of n (n > 0). */
    idx_t t = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0ULL)
    {
        ++t;
        m >>= 1;
    }

    /* Compute the shifted value and the added sequence of ones. */
    /* Use 64‑bit arithmetic for the intermediate computation to avoid
       overflow when n and t are small enough (which is always the case
       for the required generation limits). */
    seq_t shift_val = (seq_t)(n << t);
    seq_t ones      = (seq_t)((1ULL << t) - 1ULL);

    return shift_val + ones;
}
