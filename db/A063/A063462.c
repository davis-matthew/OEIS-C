/*
 * A063462  a(n) = n * last digit of n.
 * https://oeis.org/A063462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n * (n mod 10)
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* convert index to seq_t */
    seq_t ld = (seq_t)(n % 10);          /* last decimal digit of n */
    return nn * ld;                      /* product is the term */
}
