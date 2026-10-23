/*
 * A047297  Numbers that are congruent to {0, 3, 4, 6} mod 7.
 * https://oeis.org/A047297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * floor((n-1)/4) + {0,3,4,6}[(n-1) mod 4]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t offsets[4] = {0, 3, 4, 6};

    /* offset 1: a(1)=0, but n>=1 is guaranteed in tests */
    if (n == 0)
        return 0;          /* guard for accidental zero */

    idx_t n1 = n - 1;       /* shift to zero‑based index */
    idx_t q   = n1 >> 2;    /* floor division by 4 */
    idx_t r   = n1 & 3;     /* remainder mod 4 */

    return (seq_t)q * (seq_t)7 + offsets[r];
}
