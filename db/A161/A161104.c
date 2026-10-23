/*
 * A161104  Number of partitions of n into nonzero triangular numbers where
 *          every part appears at least 3 times.
 * https://oeis.org/A161104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into nonzero triangular numbers
 *            * where each part appears at least 3 times
 * Range:     u8 n <= 80, u16 n <= 254, u32 n <= 981, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, calloc, free */
#include <stdbool.h>  /* bool */

seq_t a(idx_t n)
{
    /* n is 1‑based in OEIS but the formula works for all n >= 0 */
    if (n == 0) return 0;

    /* Allocate two DP arrays of size n+1.  Use C++ new when compiling as C++,
       otherwise use calloc/malloc.  The allocated objects are either trivially
       constructible (integral) or are default‑constructed by new. */
#if defined(__cplusplus)
    size_t N = (size_t)n + 1;
    seq_t *dp    = new seq_t[N]();
    seq_t *newdp = new seq_t[N]();
    seq_t *vals  = new seq_t[N]();
    seq_t *pref  = new seq_t[N]();
#else
    size_t N = (size_t)n + 1;
    seq_t *dp    = (seq_t*)calloc(N, sizeof(seq_t));
    seq_t *newdp = (seq_t*)malloc(N * sizeof(seq_t));
    seq_t *vals  = (seq_t*)malloc(N * sizeof(seq_t));
    seq_t *pref  = (seq_t*)malloc(N * sizeof(seq_t));
#endif

    if (!dp || !newdp || !vals || !pref) {
#if defined(__cplusplus)
        delete[] dp; delete[] newdp; delete[] vals; delete[] pref;
#else
        free(dp); free(newdp); free(vals); free(pref);
#endif
        return 0;
    }

    /* dp[0] = 1  (empty partition).  The rest are zero by calloc or default init. */
    dp[0] = (seq_t)1;

    /* Enumerate all triangular numbers t <= n.  t = k*(k+1)/2 */
    for (idx_t k = 1; ; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > n) break;

        /* For each residue class r modulo t, compute the contribution
           of the generating function 1 + x^{3t} + x^{4t} + ...            */
        for (idx_t r = 0; r < t && r <= n; ++r) {
            /* Determine the length of this residue sequence */
            idx_t idx = r;
            size_t len = 0;
            while (idx <= n) {
                vals[len++] = dp[idx];
                idx += t;
            }

            /* Prefix sums of vals.  pref[i] = sum_{j=0}^{i} vals[j] */
            if (len > 0) {
                pref[0] = vals[0];
                for (size_t i = 1; i < len; ++i) {
                    pref[i] = pref[i-1] + vals[i];
                }
            }

            /* Build newdp for this residue */
            for (size_t i = 0; i < len; ++i) {
                idx_t pos = r + (idx_t)i * t;
                newdp[pos] = vals[i];
                if (i >= 3) {
                    newdp[pos] += pref[i-3];
                }
            }
        }

        /* Swap dp and newdp for the next triangular number */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];

    /* Free memory */
#if defined(__cplusplus)
    delete[] dp; delete[] newdp; delete[] vals; delete[] pref;
#else
    free(dp); free(newdp); free(vals); free(pref);
#endif

    return result;
}
