/*
 * A228306  The Wiener index of the Kneser graph K(n,2) (n>=5).
 * https://oeis.org/A228306
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n+5)/8
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 429, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;           /* offset starts at 5; value below is undefined */
    seq_t N = n;                    /* promote to seq_t before arithmetic */
    seq_t term = (N * (N - 1) * (N - 2) * (N + 5)) / 8;
    return term;
}
