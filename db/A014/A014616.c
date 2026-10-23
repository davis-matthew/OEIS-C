/*
 * A014616  a(n) = solution to the postage stamp problem with 2 denominations
 *          and n stamps.
 * https://oeis.org/A014616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor((n^2 + 6*n + 1)/4)
 * Range:     u8 n <= 15, u16 n <= 508, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t nt = (seq_t)n;
    seq_t n2 = nt * nt;
    seq_t n6 = (seq_t)6 * nt;
    /* Compute the numerator and divide by 4 */
    seq_t val = (n2 + n6 + (seq_t)1) / 4;
    return val;
}
