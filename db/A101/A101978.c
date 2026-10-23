/*
 * A101978  Number of powers of factorials not exceeding n!.
 * https://oeis.org/A101978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=2}^{n} floor( log(n!) / log(k!) )
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where log denotes the natural logarithm.  The sequence counts the
   distinct powers of factorials ≤ n!.  For n = 1 the value is 1. */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        /* The OEIS offset is 1; if called with n < 1 the behavior is
           undefined, but we return 0 to avoid crashes. */
        return 0;
    }

    /* Pre‑compute logarithms of factorials: logfac[i] = log(i!) for i=0..n. */
    long double *logfac = (long double *)malloc((size_t)(n + 1) * sizeof(long double));
    if (!logfac) {
        /* Allocation failure – return 0 as a safe fallback. */
        return 0;
    }

    logfac[0] = 0.0L;
    for (idx_t i = 1; i <= n; ++i) {
        logfac[i] = logfac[i - 1] + logl((long double)i);
    }

    /* Start with 1 for the value 1 = (1!)^j. */
    seq_t total = 1;

    const long double ln_nfac = logfac[n];
    for (idx_t k = 2; k <= n; ++k) {
        /* The largest integer j with (k!)^j <= n! satisfies
           j * log(k!) <= log(n!), hence j = floor( ln_nfac / logfac[k] ). */
        long double raw = ln_nfac / logfac[k];
        /* Add a tiny epsilon to guard against round‑off just below an integer. */
        idx_t j = (idx_t)(raw + 1e-12L);
        if (j < 1) j = 1;          /* (k!)^1 always ≤ n! when k ≤ n */
        total += (seq_t)j;
    }

    free(logfac);
    return total;
}
