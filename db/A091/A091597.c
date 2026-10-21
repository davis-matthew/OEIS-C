/*
 * A091597  Triangle read by rows: T(n,0) = A001045(n+1), T(n,n)=1, T(n,m) =
 *          T(n-1,m-1) + T(n-1,m).
 * https://oeis.org/A091597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r+1)/2, T(r,0) = J_{r+1} (Jacobsthal numbers: J_0=0, J_1=1,
 *            J_k = J_{k-1}+2*J_{k-2}), T(r,r) = 1, T(r,m) = T(r-1,m-1) +
 *            T(r-1,m)
 * Range:     u8 n <= 44, u16 n <= 152, u32 n <= 560, u64 n <= 2144,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  For the maximum n the row index r satisfies r(r+1)/2 <= n.
        For n up to 10 000, r never exceeds 140.  A fixed small
        static buffer is sufficient and avoids any non‑trivial
        construction issues. */
    const idx_t MAXR = 200;          /* safely > 140 */
    seq_t J[MAXR];                   /* Jacobsthal numbers */
    seq_t prev[MAXR];
    seq_t curr[MAXR];

    /* Find row r and column c of index n. */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n) ++r;
    r -= 1;                         /* exact row containing n */
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;             /* 0‑based column in the row */

    /* Pre‑compute Jacobsthal numbers up to J_{r+1}. */
    J[0] = (seq_t)0;
    J[1] = (seq_t)1;
    for (idx_t k = 2; k <= r + 1; ++k)
        J[k] = J[k - 1] + ((seq_t)2 * J[k - 2]);

    /* Build the triangle row by row until we reach row r. */
    prev[0] = J[1];                  /* row 0 */
    if (r == 0)                      /* special case */
        return prev[0];

    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = J[i + 1];          /* first element of row i */
        for (idx_t j = 1; j < i; ++j)
            curr[j] = prev[j - 1] + prev[j];  /* Pascal recurrence */
        curr[i] = (seq_t)1;          /* last element */

        if (i == r)                 /* we have the requested row */
            return curr[c];

        /* Copy current row into prev for the next iteration. */
        for (idx_t j = 0; j <= i; ++j)
            prev[j] = curr[j];
    }

    /* Should never reach here */
    return (seq_t)0;
}
