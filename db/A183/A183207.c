/*
 * A183207  Termwise products of the natural numbers and odd integers
 *          repeated.
 * https://oeis.org/A183207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - n*(1+(-1)^n)/2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* For even n, (-1)^n = 1 → subtract n; for odd, subtract 0 */
    return (n & 1) ? sn * sn : sn * (sn - 1);
}
