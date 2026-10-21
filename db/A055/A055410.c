/*
 * A055410  Number of points in Z^4 of norm <= n.
 * https://oeis.org/A055410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 8 * Σ_{d=1}^{n², 4∤d} d * ⌊n² / d⌋
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 171, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of integer lattice points in ℤ⁴ with Euclidean norm ≤ n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* only the origin */

    seq_t N = (seq_t)n * (seq_t)n;   /* n² */
    seq_t sum = 0;

    for (idx_t l = 1; l <= N; )
    {
        seq_t q = N / (seq_t)l;               /* floor(N / l) */
        idx_t r = (idx_t)(N / (idx_t)q);      /* largest d with floor(N/d)=q */

        seq_t cnt      = (seq_t)(r - l + 1);
        seq_t sumRange = ((seq_t)l + (seq_t)r) * cnt / 2;

        /* subtract the contribution of d divisible by 4 */
        seq_t sumDiv4 = 0;
        idx_t first4 = ((l + 3) / 4) * 4;     /* first multiple of 4 ≥ l */
        if (first4 <= r)
        {
            idx_t last4 = (r / 4) * 4;        /* last multiple of 4 ≤ r */
            seq_t k1 = first4 / 4;
            seq_t k2 = last4 / 4;
            seq_t m  = k2 - k1 + 1;
            sumDiv4 = 4 * (k1 + k2) * m / 2; /* arithmetic progression sum */
        }

        sum += (sumRange - sumDiv4) * q;
        l = r + 1;
    }

    sum = sum * (seq_t)8 + (seq_t)1;
    return sum;
}
