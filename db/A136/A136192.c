/*
 * A136192  Primes p such that 2p-3 and 2p+3 are both prime (A092110), with
 *          last decimal of p being 7.
 * https://oeis.org/A136192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 86, u32 n <= 87, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A136192: primes p such that 2p‑3 and 2p+3 are both prime
 * and the decimal representation of p ends in 7.  The sequence is
 * evaluated on demand and memoised in a static cache so that calls
 * need not be in increasing order.  The implementation uses the helper
 * function is_prime_big to test primality for arbitrary‑precision
 * seq_t values.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

/* Static cache storing discovered terms of the sequence. */
static seq_t *cache     = NULL;   /* array of terms */
static size_t cache_sz  = 0;      /* number of terms currently stored */
static size_t cache_cap = 0;      /* capacity of the cache */

/* Ensure the cache can hold at least 'need' terms. */
static void
ensure_cache_size(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* allocation failure – should not happen */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Main sequence function.  An O(k) scan is performed each time a new
 * term is required, but results are cached for future calls. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1; guard value */

    /* Return cached value if already computed. */
    if (cache_sz >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Generate terms until we have at least n of them. */
    seq_t candidate = 7;          /* first prime ending in 7 */

    /* If we had previously generated some terms, resume from the last
     * candidate that was tried rather than starting at 7 again. */
    if (cache_sz > 0) {
        /* The last accepted term is cache[cache_sz-1].
         * The next candidate should be this + 10. */
        candidate = cache[cache_sz - 1];
        candidate = candidate + 10;
    }

    while (cache_sz < (size_t)n) {
        /* Only numbers ending in 7 are relevant.  Test primality of the
         * candidate itself and of 2*p-3 and 2*p+3. */
        if (is_prime_big(candidate)) {
            seq_t q1 = 2 * candidate - 3;
            seq_t q2 = 2 * candidate + 3;
            if (is_prime_big(q1) && is_prime_big(q2)) {
                /* Store the new term. */
                ensure_cache_size(cache_sz + 1);
                cache[cache_sz++] = candidate;
            }
        }
        candidate = candidate + 10;          /* next number ending with 7 */
    }

    return cache[(size_t)n - 1];
}
