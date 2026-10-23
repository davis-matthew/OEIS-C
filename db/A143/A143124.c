/*
 * A143124  Triangle read by rows: T(n,k) = Sum_{j=k..n} A001462(j), 1<=k<=n,
 *          A001462 = Golomb's sequence.
 * https://oeis.org/A143124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{j=c(n)..r(n)} A001462(j) where r = ceil((√(8n+1)-1)/2)
 *            and c = n - r(r-1)/2
 * Range:     u8 n <= 666, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    static seq_t *g   = NULL;   /* g[1..gmax] (g[0] unused)      */
    static seq_t *S   = NULL;   /* S[0..gmax], S[0] = 0          */
    static idx_t  gmax = 0;     /* largest index currently cached */

    /* ---------- compute row r and column c for position n ---------- */
    idx_t r;
    {
        /* r satisfies r(r+1)/2 ≥ n and (r-1)r/2 < n  */
        double dn = (double)n;
        r = (idx_t)((sqrt(8.0*dn + 1.0) - 1.0) / 2.0);
        if ((r*(r+1)) / 2 < n)
            ++r;
    }
    idx_t c = n - (r*(r-1))/2;   /* 1‑based column within row r  */

    /* ---------- ensure cache covers indices up to r ---------- */
    if (r > gmax) {
        idx_t newmax = r;

        /* reallocate g and S to hold indices 0..newmax          */
        seq_t *tmp_g = (seq_t *)realloc(g, (newmax + 1) * sizeof(seq_t));
        seq_t *tmp_S = (seq_t *)realloc(S, (newmax + 1) * sizeof(seq_t));
        if (!tmp_g || !tmp_S) {
            free(tmp_g);
            free(tmp_S);
            return 0;  /* allocation failure → 0 as fallback */
        }
        g = tmp_g;
        S = tmp_S;

        S[0] = 0;

        /* compute Golomb numbers and prefix sums for missing indices */
        for (idx_t i = gmax + 1; i <= newmax; ++i) {
            if (i == 1) {
                g[i] = 1;
            } else {
                /* indices inside the recurrence must be integers      */
                idx_t prev   = (idx_t)g[i-1];
                idx_t inner  = (idx_t)g[prev];
                g[i] = 1 + g[i - inner];
            }
            S[i] = S[i-1] + g[i];
        }
        gmax = newmax;
    }

    /* ---------- return the required triangle entry ---------- */
    return S[r] - S[c-1];
}
