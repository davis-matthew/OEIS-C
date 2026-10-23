/*
 * A132442  Triangle whose n-th row consists of the first n terms of the n-th
 *          row of A134866.
 * https://oeis.org/A132442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = σ(gcd(r,k)), where n = ½(r−1)r + k (1 ≤ k ≤ r)
 * Range:     u8 n <= 5885, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Identify the row r and column k that correspond to the
       nth element in the triangular arrangement. */
    idx_t r   = 1;
    idx_t cum = 1;          /* number of elements up through row r */
    while (cum < n)         /* find the minimal r with cum ≥ n */
    {
        ++r;
        cum += r;
    }
    idx_t k = n - (cum - r);  /* column index in the current row */

    /* Euclidean algorithm for gcd(r,k). */
    idx_t a_tmp = r, b_tmp = k;
    while (b_tmp != 0)
    {
        idx_t t = a_tmp % b_tmp;
        a_tmp   = b_tmp;
        b_tmp   = t;
    }
    idx_t g = a_tmp;          /* gcd(r,k) */

    /* Compute σ(g) – sum of all positive divisors of g. */
    seq_t sigma = 0;
    for (idx_t i = 1; i * i <= g; ++i)
    {
        if (g % i == 0)
        {
            sigma += (seq_t)i;
            idx_t j = g / i;
            if (j != i)
                sigma += (seq_t)j;
        }
    }
    return sigma;
}
