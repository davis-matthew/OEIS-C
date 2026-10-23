/*
 * A180316  Concatenation of n and A008954(n).
 * https://oeis.org/A180316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = n * 10 + ((n * (n + 1) / 2) % 10)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenation of n and the last decimal digit of n(n+1)/2  */

seq_t a(idx_t n)
{
    /* last decimal digit of the triangular number T_n = n*(n+1)/2 */
    /* compute modulo 20 first to avoid overflow when seq_t is small */
    unsigned dig = (( (n % 20) * ((n + 1) % 20) ) / 2) % 10;
    return (seq_t)n * (seq_t)10 + (seq_t)dig;
}
