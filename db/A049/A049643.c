/*
 * A049643  Number of fractions in Farey series of order n.
 * https://oeis.org/A049643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + Σ_{k=1..n} φ(k) (a(0)=0)
 * Range:     u8 n <= 28, u16 n <= 463, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Largest index currently cached (global state). */
static idx_t cache_cap = 0;

#ifdef __cplusplus
/* Dynamically sized C++ vector for cumulative sums in C++ builds. */
static std::vector<seq_t> cum_vec;
#else
/* Raw pointer for C builds. */
static seq_t *cum = NULL;
#endif

/* Extend the cache to encompass all indices up to new_cap (inclusive). */
static void extend_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;   /* nothing new to compute */

    /* ----------------------------------------------------------------------------
       1. Compute Euler's totient values φ(1…new_cap) via a simple sieve. ----------
       2. Build the cumulative sum array cum[i] = Σ_{k=1..i} φ(k).
       ---------------------------------------------------------------------------- */

    /* Allocate temporary array for φ values. */
    uint32_t *phi = (uint32_t *)malloc((size_t)(new_cap + 1) * sizeof(uint32_t));
    if (!phi)
        exit(1);

    for (idx_t i = 0; i <= new_cap; ++i)
        phi[i] = (uint32_t)i;

    for (idx_t p = 2; p <= new_cap; ++p) {
        if (phi[p] == (uint32_t)p) {               /* p is prime */
            for (idx_t j = p; j <= new_cap; j += p)
                phi[j] -= phi[j] / (uint32_t)p;
        }
    }

#ifdef __cplusplus
    /* Resize the C++ vector and fill it with the cumulative sums. */
    cum_vec.resize(new_cap + 1);
    cum_vec[0] = (seq_t)0;
    for (idx_t i = 1; i <= new_cap; ++i)
        cum_vec[i] = cum_vec[i-1] + (seq_t)phi[i];
#else
    /* Allocate raw array for cumulative sums. */
    seq_t *new_cum = (seq_t *)malloc((size_t)(new_cap + 1) * sizeof(seq_t));
    if (!new_cum)
        exit(1);

    new_cum[0] = (seq_t)0;
    for (idx_t i = 1; i <= new_cap; ++i)
        new_cum[i] = new_cum[i-1] + (seq_t)phi[i];

    free(cum);   /* free the old array (NULL works). */
    cum = new_cum;
#endif

    free(phi);
    cache_cap = new_cap;
}

/* Begonia sequence A049643: number of fractions in Farey series of order n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* a(0) = 0 */

    extend_cache(n);

#ifdef __cplusplus
    return cum_vec[n] + (seq_t)1;   /* a(n) = 1 + Σ_{k=1..n} φ(k) */
#else
    return cum[n] + (seq_t)1;
#endif
}
