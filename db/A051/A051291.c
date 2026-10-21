/*
 * A051291  Whitney number of level n of the lattice of the ideals of the
 *          fence of order 2*n + 1.
 * https://oeis.org/A051291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1..⌊(n+1)/2⌋} binomial(n‑d+2, d) * binomial(n‑d,
 *            d‑1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset. */
    if (n == 0u)
        return (seq_t)1;          /* a(0) = 1 */

    /* Number of terms in the sum. */
    const idx_t m = (n + 1) / 2;   /* floor((n+1)/2) */

    /* For d = 1 we can initialise the two binomial coefficients directly. */
    seq_t C1 = (seq_t)(n + 1);      /* binomial(n‑1+2, 1) = n + 1 */
    seq_t C2 = (seq_t)1;            /* binomial(n‑1, 0) = 1 */

    seq_t sum = C1 * C2;            /* contribution of d = 1 */

    /* Iterate d = 1 .. m‑1 to compute the remaining terms. */
    for (idx_t d = 1; d < m; ++d)
    {
        /* Compute C1 for d+1:
           C1_{d+1} = C1_d * (n‑2d+2)/(n‑d+2) * (n‑2d+1)/(d+1)
        */
        seq_t nextC1 = C1;
        nextC1 *= (seq_t)(n - 2 * d + 2);
        nextC1 /= (seq_t)(n - d + 2);
        nextC1 *= (seq_t)(n - 2 * d + 1);
        nextC1 /= (seq_t)(d + 1);

        /* Compute C2 for d+1:
           C2_{d+1} = C2_d * (n‑2d+1)/d * (n‑2d)/(n‑d)
        */
        seq_t nextC2 = C2;
        nextC2 *= (seq_t)(n - 2 * d + 1);
        nextC2 /= (seq_t)d;
        nextC2 *= (seq_t)(n - 2 * d);
        nextC2 /= (seq_t)(n - d);

        /* Update for next iteration. */
        C1 = nextC1;
        C2 = nextC2;

        /* Accumulate the product for the new d. */
        sum += C1 * C2;
    }

    return sum;
}
