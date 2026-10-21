/*
 * A066705  Greatest k < n such that phi(k) = phi(n) if such k exists,
 *          otherwise 0.
 * https://oeis.org/A066705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest k < n such that φ(k) = φ(n); a(n) = 0 if no such
 *            k exists. The function caches φ(i) for i ≤ current maximum n so
 *            that each call is O(n) or better with precomputation
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of totient values, using uint64_t so that all comparisons are exact. */
static uint64_t *phi_cache = NULL;          /* phi_cache[i] = φ(i) for 0 ≤ i ≤ max_n */
static idx_t max_precomputed_n = 0;         /* highest n for which cache is valid */

/* Resize φ cache to at least size n+1 and compute φ values up to n. */
static void ensure_phi(idx_t n)
{
    if (n <= max_precomputed_n)
        return;

    /* Reallocate cache to new size */
    uint64_t *new_cache = (uint64_t*)realloc(phi_cache, (n + 1) * sizeof(uint64_t));
    if (!new_cache) {
        /* Out of memory: fail gracefully by aborting */
        abort();
    }
    phi_cache = new_cache;

    /* Initialize φ[i] = i for all indices up to n. */
    for (idx_t i = 0; i <= n; ++i)
        phi_cache[i] = (uint64_t)i;

    /* Euler's totient sieve. */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi_cache[p] == (uint64_t)p) {       /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                phi_cache[j] -= phi_cache[j] / p;
        }
    }

    max_precomputed_n = n;
}

/* Return the A066705 value for index n.  The offset is 1 (a(1)=0). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, return 0 */

    ensure_phi(n);

    uint64_t phi_n = phi_cache[n];
    for (idx_t k = n - 1; k > 0; --k) {
        if (phi_cache[k] == phi_n)
            return (seq_t)k;       /* found greatest k < n with equal φ */
    }
    return (seq_t)0;               /* none found */
}
