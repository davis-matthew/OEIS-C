/*
 * A212942  Number of nX7 0..6 arrays with no column j greater than or equal
 *          to than column j-1 in all rows.
 * https://oeis.org/A212942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ(-1)^|T| · ( Π_{chains of T} C(L+6,6) ) ^ n where T ⊆
 *            {1,…,6} indexes the 6 adjacent column pairs. C(k,6) = k! / (6!
 *            (k-6)!) and L is the length of a chain of columns that must be
 *            non‑decreasing. C( L+6,6 ) is 7,28,84,210,462,924,1716 for L =
 *            1…7. See the OEIS A212942 entry for the deriva...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pre‑compute per‑row counts for all 64 subsets of the 6 column
       adjacencies.  The values fit into a seq_t even for the
       unconstrained case (7^7 = 823543). */
    static seq_t per_row[64];
    static int initialized = 0;
    if (!initialized) {
        static const seq_t comb[8] = {0,7,28,84,210,462,924,1716};
        for (int mask = 0; mask < 64; ++mask) {
            int len = 1;          /* current chain length   */
            seq_t prod = 1;       /* product of chain counts */
            for (int j = 1; j <= 6; ++j) {
                if (mask & (1 << (j-1))) {   /* constraint present */
                    len++;
                } else {                   /* chain ends          */
                    prod *= comb[len];
                    len = 1;
                }
            }
            prod *= comb[len];              /* last chain          */
            per_row[mask] = prod;
        }
        initialized = 1;
    }

    seq_t pos = 0;     /* sum of terms with even number of constraints */
    seq_t neg = 0;     /* sum of terms with odd  number of constraints */

    for (int mask = 0; mask < 64; ++mask) {
        seq_t b = per_row[mask];
        /* fast exponentiation: b^n */
        seq_t term = 1;
        seq_t curr = b;
        idx_t e = n;
        while (e) {
            if (e & 1) term *= curr;
            e >>= 1;
            if (e) curr *= curr;
        }

        int bits = __builtin_popcount(mask);
        if ((bits & 1) == 0)
            pos += term;
        else
            neg += term;
    }

    return pos - neg;   /* pos ≥ neg for all n ≥ 1 */
}
