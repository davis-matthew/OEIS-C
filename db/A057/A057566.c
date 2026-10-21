/*
 * A057566  Number of collinear triples in a 3 X n rectangular grid.
 * https://oeis.org/A057566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 - 4*n^2 + 4*n + (n mod 2 ? 2 : 0)) / 4
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1839, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;          /* convert index to seq_t for arithmetic   */
    seq_t term = 2 * nn * nn * nn;  /* 2*n³  */
    term -= 4 * nn * nn;            /* -4*n² */
    term += 4 * nn;                 /* +4*n  */
    if (n & 1) term += 2;           /* add 2 if n is odd (corresponds to +½) */
    return term / 4;                /* divide by 4 to get integer result       */
}
