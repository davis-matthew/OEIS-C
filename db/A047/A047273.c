/*
 * A047273  Numbers that are congruent to {0, 1, 3, 5} mod 6.
 * https://oeis.org/A047273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor((n-1)/4) + {0,1,3,5}[(n-1)%4]
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t k = n - 1;          /* shift to 0‑based */
    idx_t q = k / 4;
    idx_t r = k % 4;
    static const seq_t prefix[4] = {0, 1, 3, 5};

    return (seq_t)6 * (seq_t)q + prefix[r];
}
