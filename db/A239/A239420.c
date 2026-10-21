/*
 * A239420  Number of nX2 0..3 arrays with no element equal to the sum of
 *          elements to its left or the sum of elements above it or the sum of
 *          the elements diagonally to its northwest, modulo 4.
 * https://oeis.org/A239420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming over cumulative sums modulo 4.
 *            The DP state is (s1, s2, prev_a1) where s1, s2 are the sums of
 *            the two columns modulo 4 after the current row, and prev_a1 is
 *            the value in the first column of the previous row (needed for
 *            the NW‑diagonal constraint). Transition: choos...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */

    /* DP tables: 4 × 4 × 4 states, each entry holds the count of ways. */
    static seq_t dp[4][4][4];
    static seq_t next[4][4][4];
    for (idx_t i = 0; i < 4; ++i)
        for (idx_t j = 0; j < 4; ++j)
            for (idx_t k = 0; k < 4; ++k)
                dp[i][j][k] = 0;

    /* --- Row 1 ----------------------------------------------------- */
    /* For the first row there is no “above” or “NW‑diagonal” element,
       so only the left‑sum constraint applies. */
    for (idx_t a1 = 1; a1 < 4; ++a1) {            /* a1 ≠ 0 */
        idx_t s1new = a1 & 3;                     /* s1' = a1 (mod 4) */
        for (idx_t a2 = 0; a2 < 4; ++a2) {
            if (a2 == a1) continue;               /* left neighbor */
            if (a2 == 0)   continue;              /* sum above = 0 */
            idx_t s2new = a2 & 3;                 /* s2' = a2 (mod 4) */
            dp[s1new][s2new][a1] += 1;
        }
    }

    /* --- Rows 2 … n ----------------------------------------------- */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero next table */
        for (idx_t i = 0; i < 4; ++i)
            for (idx_t j = 0; j < 4; ++j)
                for (idx_t k = 0; k < 4; ++k)
                    next[i][j][k] = 0;

        for (idx_t s1 = 0; s1 < 4; ++s1)
            for (idx_t s2 = 0; s2 < 4; ++s2)
                for (idx_t prev_a1 = 0; prev_a1 < 4; ++prev_a1) {
                    seq_t ways = dp[s1][s2][prev_a1];
                    if (ways == 0) continue;

                    /* Choose a1 for this row. */
                    for (idx_t a1 = 0; a1 < 4; ++a1) {
                        if (a1 == 0) continue;         /* left sum = 0 */
                        if (a1 == s1) continue;        /* sum above */
                        idx_t s1new = (s1 + a1) & 3;    /* new cumulative */
                        /* Choose a2 for this row. */
                        for (idx_t a2 = 0; a2 < 4; ++a2) {
                            if (a2 == a1)     continue; /* left neighbor */
                            if (a2 == s2)     continue; /* sum above */
                            if (a2 == prev_a1)continue; /* NW diagonal */
                            idx_t s2new = (s2 + a2) & 3;  /* new cumulative */
                            next[s1new][s2new][a1] += ways;
                        }
                    }
                }

        /* Prepare for next iteration. */
        for (idx_t i = 0; i < 4; ++i)
            for (idx_t j = 0; j < 4; ++j)
                for (idx_t k = 0; k < 4; ++k)
                    dp[i][j][k] = next[i][j][k];
    }

    /* --- Sum all final states ------------------------------------- */
    seq_t total = 0;
    for (idx_t s1 = 0; s1 < 4; ++s1)
        for (idx_t s2 = 0; s2 < 4; ++s2)
            for (idx_t prev_a1 = 0; prev_a1 < 4; ++prev_a1)
                total += dp[s1][s2][prev_a1];
    return total;
}
