/*
 * A036443  Number of ternary rooted trees with n nodes and height exactly 3.
 * https://oeis.org/A036443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of ternary rooted trees with n nodes and height exactly 3. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= 3; ++k)          /* number of children of the root (1‑3) */
    {
        for (idx_t I = 1; I <= k; ++I)       /* number of internal depth‑2 children (must be ≥1) */
        {
            /* distribute the I internal nodes among the 3 possible leaf counts */
            for (idx_t a1 = 0; a1 <= I; ++a1)     /* internal nodes having 1 leaf */
            {
                for (idx_t a2 = 0; a2 <= I - a1; ++a2)  /* internal nodes having 2 leaves */
                {
                    idx_t a3 = I - a1 - a2;     /* internal nodes having 3 leaves */
                    /* total nodes: root (1) + k depth‑2 nodes + leaves at depth‑3
                     * leaves count = a1 + 2*a2 + 3*a3
                     */
                    idx_t nodes = 1 + k + a1 + (2 * a2) + (3 * a3);
                    if (nodes == n)
                        ++count;
                }
            }
        }
    }
    return count;
}
