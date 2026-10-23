/*
 * A156697  Triangle T(n, k, m) = t(n,m)/( t(k,m) * t(n-k,m) ) with T(n, 0, m)
 *          = T(n, n, m) = 1, where t(n, m) = Product_{j=1..n}
 *          Product_{i=1..j-1} ( 1 - (m+1)*(2*i-1) ) and m = 2, read by rows.
 * https://oeis.org/A156697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the nth entry of the triangle T(n,k,2) read row‑by‑row.
 *            For the triangle T(r,k,2) = t(r) / ( t(k) * t(r-k) ), 0 ≤ k ≤ r,
 *            with t(m) = ∏_{i=1}^{m-1} (4-6i)^{(m-i)}
 * Range:     i8 n <= 10, i16 n <= 16, i32 n <= 23, i64 n <= 39, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for arbitrary‑precision seq_t and unsigned idx_t. */
seq_t a(idx_t n)
{
    /* Find the row r such that the first n elements are in rows 0 .. r‑1,
       i.e. cumulative elements N(r‑1) ≤ n < N(r), where
       N(r) = (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* The position k within the row. */
    idx_t prev = (r * (r + 1)) / 2;          /* N(r‑1) */
    idx_t k = n - prev;                      /* 0 ≤ k ≤ r */

    /* T(r,k,2) is 1 when k==0 or k==r. */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Compute the exponents e_i for i = 1 .. r‑1 as described: */
    /*   e_i = i                 if i ≤ min(k‑1,r-k‑1)
     *   e_i = r‑k               if i > k‑1  and i ≤ r‑k‑1
     *   e_i = k                 if i > r‑k‑1 and i ≤ k‑1
     *   e_i = r‑i               otherwise   */
    idx_t l = k - 1;           /* upper bound for k‑1   */
    idx_t m = r - k - 1;       /* upper bound for r‑k‑1 */

    seq_t result = (seq_t)1;

    for (idx_t i = 1; i < r; ++i)
    {
        int e = 0;
        if (i <= l && i <= m)
            e = (int)i;
        else if (i <= l)
            e = (int)(r - k);
        else if (i <= m)
            e = (int)k;
        else
            e = (int)(r - i);

        if (e == 0)
            continue;

        /* base = 4 – 6i, always a negative integer. */
        seq_t base = (seq_t)4 - (seq_t)6 * (seq_t)i;

        seq_t pow_val = (seq_t)1;
        for (int t = 0; t < e; ++t)
            pow_val *= base;

        result *= pow_val;
    }

    return result;
}
