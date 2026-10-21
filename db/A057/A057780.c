/*
 * A057780  Multiples of 3 that are one less than a perfect square.
 * https://oeis.org/A057780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + (n-1)/2)^2 - 1) where n is the OEIS index starting
 *            at 1. The expression (n + (n-1)/2) produces the n‑th positive
 *            integer not divisible by 3, and squaring it minus one gives the
 *            next multiple of 3 that is one less than a perfect square
 * Range:     u8 n <= 11, u16 n <= 171, u32 n <= 2001, u64 n <= 2001,
 *            u128 n <= 2001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ensure n >= 1 (offset 1), cast to seq_t for arithmetic */
    seq_t ns = (seq_t)n;
    /* find the n-th integer that is not divisible by 3:
       m = n + floor((n-1)/2) */
    seq_t m = ns + (ns - (seq_t)1) / (seq_t)2;
    /* compute m^2 - 1 */
    return m * m - (seq_t)1;
}
