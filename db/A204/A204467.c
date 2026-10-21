/*
 * A204467  Number of 3-element subsets that can be chosen from
 *          {1,2,...,6*n+3} having element sum 9*n+6.
 * https://oeis.org/A204467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n*(3*n+2)+(-1)^n+3)/4 (OEIS A204467)
 * Range:     u8 n <= 3, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use seq_t for the entire calculation to avoid overflow in any build */
    seq_t nn   = (seq_t)n;                         /* n in seq_t domain */
    seq_t temp = (seq_t)6 * nn * ((seq_t)3 * nn + 2); /* 6*n*(3*n+2) */

    /* (-1)^n is +1 for even n, -1 for odd n.
       The constant term added is alternate between +4 (even) and +2 (odd).
    */
    seq_t add = (n & 1) ? 2 : 4;                   /* 2 if n odd, 4 if n even */

    return (temp + add) / 4;
}
