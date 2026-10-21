/*
 * A070535  a(n) = n^4 mod 18.
 * https://oeis.org/A070535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 18
 * Range:     u8 n <= 272, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t  = (seq_t)n % 18;          /* reduce n modulo 18 */
    seq_t t2 = (t * t) % 18;           /* n^2 mod 18 */
    seq_t t4 = (t2 * t2) % 18;         /* n^4 mod 18 */
    return t4;
}
