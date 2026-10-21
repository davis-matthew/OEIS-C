/*
 * A076312  a(n) = floor(n/10) + 2*(n mod 10).
 * https://oeis.org/A076312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/10) + 2*(n mod 10)
 * Range:     u8 n <= 507, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t to perform arithmetic in the proper type */
    seq_t ns  = (seq_t)n;
    seq_t div = ns / (seq_t)10;
    seq_t mod = ns % (seq_t)10;
    return div + (mod * 2);
}
