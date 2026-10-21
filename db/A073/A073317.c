/*
 * A073317  a(n)=Sum_{j = 0..n} binomial(phi(n),phi(j)).
 * https://oeis.org/A073317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{j = 0..n} binomial(phi(n), phi(j))
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Cache for Euler's totient values φ(i) for i in [0, phi_cache_size-1] */
static idx_t *phi_cache = NULL;
static idx_t phi_cache_size = 0;

/* Recompute φ(i) for all i ≤ limit and resize the cache accordingly. */
static void recompute_phi(idx_t limit)
{
    /* Allocate new cache (size limit+1). */
    idx_t *new_cache = (idx_t *)realloc(phi_cache, (limit + 1) * sizeof(idx_t));
    if (new_cache == NULL) abort();      /* allocation failure */
    phi_cache = new_cache;

    /* Initialize φ(i) = i. */
    for (idx_t i = 0; i <= limit; ++i)
        phi_cache[i] = i;

    /* Sieve to subtract multiples of each prime. */
    for (idx_t p = 2; p <= limit; ++p) {
        if (phi_cache[p] == p) {          /* p is prime */
            for (idx_t k = p; k <= limit; k += p)
                phi_cache[k] -= phi_cache[k] / p;
        }
    }

    /* φ(0) = 0, φ(1) = 1 by definition */
    phi_cache[0] = 0;
    if (limit >= 1)
        phi_cache[1] = 1;

    phi_cache_size = limit + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* safety for indices < 1 */

    /* Ensure φ values up to n are cached. */
    if (n >= phi_cache_size)
        recompute_phi(n);

    idx_t phi_n = phi_cache[n];
    seq_t sum = (seq_t)0;

    for (idx_t j = 0; j <= n; ++j) {
        idx_t phi_j = phi_cache[j];
        sum += binomial(phi_n, phi_j);     /* binomial returns seq_t */
    }

    return sum;
}
