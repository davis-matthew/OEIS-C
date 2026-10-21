/*
 * A073985  Number of strings of length n over Z_6 with trace 2 and subtrace
 *          2.
 * https://oeis.org/A073985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A073985
    Number of strings of length n over Z_6 with trace 2 and subtrace 2. 
    Computation is performed by dynamic programming on trace and subtrace
    modulo 6, which has only 36 states.  The table is propagated for
    each length up to the required n.  @a[n] holds the value
    a(n;2,2).  Complexity: O(36·6·n) ≈ O(n).  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static idx_t max_n = 0;          /* largest n already computed   */
    static seq_t f[6][6]          = {}; /* counts for current length   */
    static seq_t g[6][6];          /* temporary array for next step */
    static seq_t *a_vals          = NULL; /* results array               */
    static idx_t a_cap            = 0;     /* capacity of a_vals          */

    /* initialise for length 0 */
    if (max_n == 0) {
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                f[i][j] = 0;
        f[0][0] = 1;                       /* empty string */

        a_cap = 16;                        /* initial capacity */
        a_vals = (seq_t*)malloc(a_cap * sizeof(seq_t));
        a_vals[0] = 0;                     /* a(0), unused but keeps indices aligned */
    }

    /* return previously computed value if possible */
    if (n <= max_n)
        return a_vals[n];

    /* compute successive lengths */
    for (idx_t cur = max_n; cur < n; ++cur) {
        /* zero next table */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                g[i][j] = 0;

        /* propagate counts */
        for (int t = 0; t < 6; ++t) {
            for (int s = 0; s < 6; ++s) {
                seq_t cnt = f[t][s];
                if (cnt == 0) continue;
                for (int x = 0; x < 6; ++x) {
                    int nt = t + x;
                    if (nt >= 6) nt -= 6;          /* (t+x) mod 6 */
                    int ns = s + x * t;
                    ns %= 6;                       /* (s + x*t) mod 6 */
                    g[nt][ns] += cnt;
                }
            }
        }

        /* copy new counts to current */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                f[i][j] = g[i][j];

        /* ensure result array has space */
        if (cur + 1 >= a_cap) {
            a_cap *= 2;
            a_vals = (seq_t*)realloc(a_vals, a_cap * sizeof(seq_t));
        }

        a_vals[cur + 1] = f[2][2];  /* a(n;2,2) */
        max_n = cur + 1;
    }

    return a_vals[n];
}
