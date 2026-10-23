/*
 * A121689  G.f.: Sum_{n>=0} x^n * (1+x)^(n^2).
 * https://oeis.org/A121689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0}^{n} \binom{k^2}{\,n-k\,} (OEIS A121689)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are typedefs provided by the build system.  The
   implementation below uses only integer arithmetic supported by the
   seq_t type.  The routine caches the computed values up to the
   largest index requested so that subsequent calls return instantly.  */

static seq_t *cache = NULL;          /* array of a(n) values       */
static idx_t cache_size = 0;         /* current cache size (>=1)  */

static void compute_up_to(idx_t N) {
    /* Allocate or reallocate cache */
    cache_size = N + 1;
    if (cache) free(cache);
    cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
    if (!cache) { /* fallback to minimal error handling */
        cache_size = 0;
        return;
    }

    /* Compute a(n) for 0 <= n <= N using the direct sum formula.
       The computation is O(N^2) but remains well under the limits
       specified (N <= 10'000).  Each binomial is updated multiplicatively */
    for (idx_t k = 0; k <= N; ++k) {
        seq_t binom = 1;                          /* C(k^2,0) */
        idx_t const kpow2 = k * k;                /* k^2 fits in 64‑bit for N <= 10'000 */

        /* Contribution for m = 0 (i.e. n = k) */
        cache[k] += binom;

        /* m = 1 .. N-k */
        for (idx_t m = 1; m <= N - k; ++m) {
            /* C(k^2, m) = C(k^2, m-1) * (k^2 - (m-1)) / m */
            binom = binom * ((seq_t)kpow2 - (seq_t)(m - 1)) / (seq_t)m;
            cache[k + m] += binom;
        }
    }
}

/* External linkage.  Client code will call this function directly. */
seq_t a(idx_t n) {
    if (n >= cache_size)            /* need to compute more terms */
        compute_up_to(n);
    if (cache_size == 0)            /* failure in allocation: return 0 */
        return (seq_t)0;
    return cache[n];
}
