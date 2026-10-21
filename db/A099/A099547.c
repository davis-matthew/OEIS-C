/*
 * A099547  Odd part of n modulo 6.
 * https://oeis.org/A099547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = oddpart(n) mod 6
 * Range:     u8 n <= 1226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* start with the integer n */
    /* remove all factors of 2 */
    while ((x & 1) == 0)         /* x is even */
        x >>= 1;                 /* divide by 2 */
    return x % 6;                /* odd part modulo 6 (1,3,5) */
}
