/*
 * A207112  Number of 3Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3xn 0..1 arrays avoiding 0 0 0 and 0 1 1
 *            horizontally and 0 0 1 and 1 1 0 vertically. Computed by dynamic
 *            programming over columns
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – value arbitrary */
    if (n == 1) return 6;          /* single column: 6 vertical‑valid patterns */
    if (n == 2) return 36;         /* two columns: all 6×6 pairs */

    /* Allowed 3‑bit column patterns (no forbidden vertical 001 or 110) */
    static const idx_t ALLOWED[6] = {0, 2, 3, 4, 5, 7};

    /* DP arrays: state indexed as prevprev*6 + prev (each 0..5) */
    seq_t dp_cur[36] = {0};
    seq_t dp_next[36];

    /* Initialize for n = 2: each pair of allowed columns contributes 1 */
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            dp_cur[i * 6 + j] = 1;

    /* Transition for columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col)
    {
        /* clear next layer */
        for (int k = 0; k < 36; ++k)
            dp_next[k] = 0;

        /* iterate over current states */
        for (int st = 0; st < 36; ++st)
        {
            seq_t ways = dp_cur[st];
            if (!ways) continue;

            int prevprev = st / 6;
            int prev     = st % 6;

            /* try every possible new column */
            for (int np = 0; np < 6; ++np)
            {
                bool ok = true;
                /* check each of the 3 rows for forbidden horizontal triples */
                for (int r = 0; r < 3 && ok; ++r)
                {
                    unsigned b2 = (ALLOWED[prevprev] >> r) & 1;
                    unsigned b1 = (ALLOWED[prev]     >> r) & 1;
                    unsigned b3 = (ALLOWED[np]       >> r) & 1;
                    unsigned mask = (b2 << 2) | (b1 << 1) | b3;
                    if (mask == 0 || mask == 3)   /* 000 or 011 forbidden */
                        ok = false;
                }
                if (ok)
                {
                    int next_state = prev * 6 + np;
                    dp_next[next_state] += ways;
                }
            }
        }

        /* move to the next column */
        for (int k = 0; k < 36; ++k)
            dp_cur[k] = dp_next[k];
    }

    /* sum all states to get total number of arrays */
    seq_t total = 0;
    for (int k = 0; k < 36; ++k)
        total += dp_cur[k];
    return total;
}
