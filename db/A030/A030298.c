/*
 * A030298  List of permutations of 1,2,3,...,n for n=1,2,3,..., in
 *          lexicographic order.
 * https://oeis.org/A030298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9590, u16 n <= 9590, u32 n <= 9590, u64 n <= 9590,
 *            u128 n <= 9590, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: concatenation of all permutations of 1..m
   in lexicographic order, for m=1,2,3,...  */
seq_t a(idx_t n)
{
    /* Find the block length m such that
       sum_{i=1}^{m-1} i*i! < n <= sum_{i=1}^{m} i*i! */
    idx_t fact = 1;          /* m!   */
    idx_t cumulative = 0;    /* sum of i*i! up to current m */
    idx_t m;

    for (m = 1; ; ++m) {
        fact = fact * m;                      /* fact = m!        */
        cumulative += fact * m;               /* add m*m! to sum */
        if (cumulative >= n)                 /* block found     */
            break;
    }

    /* cumulative now holds S(m)
       S(m-1) = cumulative - m*m!           */
    idx_t cumPrev = cumulative - fact * m;
    idx_t offset = n - cumPrev;             /* 1-based inside block */

    /* Determine which permutation and which element inside it */
    idx_t permIndex = (offset - 1) / m;      /* 0-based permutation index */
    idx_t inPerm   = (offset - 1) % m;      /* 0-based index inside permutation */

    /* Precompute factorial numbers up to m */
    idx_t factArr[32];
    factArr[0] = 1;
    for (idx_t i = 1; i <= m; ++i)
        factArr[i] = factArr[i - 1] * i;

    /* Generate the permIndex‑th permutation of [1..m] */
    int seq[32];
    for (idx_t i = 0; i < m; ++i)
        seq[i] = (int)(i + 1);

    idx_t len = m;
    idx_t remaining = permIndex;

    int perm[32];
    for (idx_t i = 0; i < m; ++i) {
        idx_t f   = factArr[m - 1 - i];      /* (m-1-i)!            */
        idx_t pos = remaining / f;          /* position in seq[]   */
        remaining %= f;

        perm[i] = seq[(int)pos];            /* pick the element    */

        /* Remove the used element from seq (shift left) */
        for (idx_t k = pos; k + 1 < len; ++k)
            seq[k] = seq[k + 1];
        --len;
    }

    /* Return the element at the desired position */
    return (seq_t)perm[(int)inPerm];
}
