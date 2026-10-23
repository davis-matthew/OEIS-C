/*
 * A176121  Triangle, read by rows, T(n,k) = binomial(n+k+1, n+1) *
 *          Sum_{j=0..k} j!*binomial(n,j)*binomial(k, j).
 * https://oeis.org/A176121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where T(n,k) = binomial(n+k+1, n+1) * Sum_{j=0..k}
 *            j!*binomial(n,j)*binomial(k,j)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 42, u64 n <= 117, u128 n <= 298,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    /* locate row r and column c (0‑based) of the linear index n */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n)
        ++r;
    --r;
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;

    seq_t bin = binomial_small(r + c + 1, r + 1);

    seq_t sum = 0;
    seq_t fact = 1;
    for (idx_t j = 0; j <= c; ++j)
    {
        if (j > 0)
            fact *= (seq_t)j;
        seq_t t = fact * binomial_small(r, j) * binomial_small(c, j);
        sum += t;
    }
    return bin * sum;
}
