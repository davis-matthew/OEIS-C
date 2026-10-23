/*
 * A120909  Triangle read by rows: T(n,k) is the number of ternary words of
 *          length n having k runs (i.e., subwords of maximal length) of
 *          identical letters (1 <= k <= n).
 * https://oeis.org/A120909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3 * 2^(k-1) * binomial(r-1, k-1)
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where (r, k) are the row and column of the n‑th entry of the triangle. */

static seq_t binom_uint64(idx_t n, idx_t k)
{
    /* Compute binomial coefficient C(n, k) for 0 <= k <= n. */
    if (k > n - k)
        k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        result = result * (seq_t)(n - (k - i));
        result = result / (seq_t)i;
    }
    return result;
}

static seq_t pow2_uint64(idx_t exp)
{
    seq_t v = 1;
    for (idx_t i = 0; i < exp; ++i)
        v = v << 1;
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* a(1) corresponds to n==1 */

    /* Find row r such that 1 + 2 + ... + r >= n. */
    idx_t r = 1;
    idx_t cum = 1;         /* cumulative number of entries up to row r */
    while (cum < n)
    {
        ++r;
        cum += r;
    }

    /* Index k within row r (1‑based). */
    idx_t k = n - (cum - r);  /* n - sum up to previous row */

    /* Compute T(r, k) = 3 * 2^(k-1) * C(r-1, k-1). */
    seq_t bin = binom_uint64(r - 1, k - 1);
    seq_t p2  = pow2_uint64(k - 1);
    seq_t res = 3 * p2;
    res *= bin;
    return res;
}
