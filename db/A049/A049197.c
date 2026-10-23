/*
 * A049197  Squarefree numbers whose Euler totient function is not squarefree.
 * https://oeis.org/A049197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree, mu_of, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049197
 * A(n) is the n-th square‑free integer k such that phi(k) is not square‑free
 * (i.e. mu(k)=±1 and mu(phi(k))=0).
 * The implementation generates the terms in increasing order by
 * incremental search and memoises the found values.  The search stops
 * exactly when the required number of terms have been produced.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_squarefree.h"
#include "../utils/phi.h"
#include "../utils/mu_of.h"

/* Find the n-th term of A049197 */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;   /* cache[i] holds the i‑th term (0‑based) */
    static size_t cache_cap = 0;   /* capacity of cache array */
    static size_t cache_len = 0;   /* number of entries currently stored */
    static idx_t last_k = 0;       /* last integer examined by the sieve */

    /* Grow the cache if needed */
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* out of memory: return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_k;                           /* next integer to test */

        /* only square‑free k are candidates */
        if (!is_squarefree((seq_t)last_k))
            continue;

        /* phi(k) is computed as a seq_t */
        seq_t ph = phi(last_k);

        /* mu_of expects an idx_t; we cast phi which fits in 64‑bit here */
        if (mu_of((idx_t)ph) == 0) {
            /* Store the new term */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;            /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    /* Return the requested term */
    return cache[n - 1];
}
