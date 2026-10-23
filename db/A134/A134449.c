/*
 * A134449  Sum of even products minus sum of odd products of different pairs
 *          of numbers from 1 to n.
 * https://oeis.org/A134449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{1≤i<j≤n} (i*j) if i or j is even, otherwise –(i*j)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 383, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed form derived from sum-of-products identities */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                     /* cast index to seq_t first */

    /* total sum of all pairwise products i*j (i<j) */
    seq_t S1   = nn * (nn + 1) / 2;          /* Σ i  */
    seq_t S2   = nn * (nn + 1) * (2 * nn + 1) / 6; /* Σ i^2 */
    seq_t total = (S1 * S1 - S2) / 2;        /* Σ_{i<j} i*j */

    /* sum over odd numbers only */
    seq_t m       = (nn + 1) / 2;            /* count of odd numbers ≤ n */
    seq_t S_odd   = m * m;                   /* Σ odd i */
    seq_t S_odd_sq = m * (2 * m - 1) * (2 * m + 1) / 3; /* Σ odd i^2 */

    /* a(n) = total – 2 * odd_prod_sum
     *       = total – (S_odd^2 – S_odd_sq)   */
    seq_t result = total - S_odd * S_odd + S_odd_sq;

    return result;
}
