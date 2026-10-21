/*
 * A070513  a(n) = n^4 mod 9.
 * https://oeis.org/A070513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 9
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 9 first, then square twice to get n^4 mod 9. */
    seq_t x = (seq_t)(n % 9);   /* n mod 9, small value */
    x = (x * x) % 9;           /* (n mod 9)^2 mod 9  */
    x = (x * x) % 9;           /* ((n mod 9)^2)^2 = n^4 mod 9 */
    return x;
}
