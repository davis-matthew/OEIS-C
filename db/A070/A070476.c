/*
 * A070476  a(n) = n^3 mod 14.
 * https://oeis.org/A070476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 mod 14
 * Range:     u8 n <= 316, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)n % (seq_t)14;          /* n mod 14, small value */
    seq_t res = (mod * mod) % (seq_t)14;       /* (n^2) mod 14  */
    return (res * mod) % (seq_t)14;            /* (n^3) mod 14  */
}
