/*
 * A159469  Maximum remainder when (k + 1)^n + (k - 1)^n is divided by k^2 for
 *          variable n and k > 2.
 * https://oeis.org/A159469
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n - n*(3 + (-1)^n)/2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined for indices below the official offset */
    seq_t sn = (seq_t)n;
    if ((n & 1u) == 0)             /* even n */
        return sn * sn - (seq_t)2 * sn;
    else                           /* odd n */
        return sn * sn - sn;
}
