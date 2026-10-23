/*
 * A008672  Expansion of 1/((1-x)*(1-x^3)*(1-x^5)).
 * https://oeis.org/A008672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor(n^2/30 + 3*n/10 + 1)
 * Range:     u8 n <= 11, u16 n <= 934, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n^2 + 9*n + 30) / 30 using integer arithmetic. */
    seq_t t = (seq_t)n * (seq_t)n;   /* n^2 */
    t += (seq_t)n * (seq_t)9;        /* + 9*n */
    t += (seq_t)30;                  /* + 30 */
    return t / (seq_t)30;            /* integer division gives floor   */
}
