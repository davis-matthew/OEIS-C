/*
 * A095668  Sixth column (m=5) of (1,4)-Pascal triangle A095666.
 * https://oeis.org/A095668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+20)*binomial(n+4,4)/5
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 213, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute C(n+4,4) = (n+4)(n+3)(n+2)(n+1)/24 */
    idx_t m = n + 4;
    seq_t bin = (seq_t)m;           /* (n+4)   */
    bin *= (m - 1);                 /* * (n+3) */
    bin *= (m - 2);                 /* * (n+2) */
    bin *= (m - 3);                 /* * (n+1) */
    bin /= 24;                      /* divide by 24 */

    /* Final formula: (n+20) * bin / 5 */
    seq_t result = (seq_t)(n + 20);
    result *= bin;
    result /= 5;
    return result;
}
