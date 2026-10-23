/*
 * A161540  Primes which are the sum of a smaller n and its composite(n)
 *          subscript. A002808 + a(n) (= prime).
 * https://oeis.org/A161540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161540: primes p = k + composite(k) where composite(k) is the
 * k‑th composite number (starting with 4).  Only those sums that are prime
 * are stored; a(n) is the n‑th such prime, indexed by order of appearance.
 * The sequence is generated lazily: we scan composite numbers, compute
 * their sum with the corresponding index, test primality, and cache the
 * results.  Subsequent calls use the cache.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the n‑th term of A161540 (1‑based index).  */
seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* capacity of the cache array */

    /* state used to generate composite numbers and the corresponding sums */
    static seq_t m    = 1;  /* current composite index (k) */
    static seq_t comp = 4;  /* the k‑th composite number */

    /* If the requested term is already cached, return it. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Grow the cache if necessary. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failed – return a safe value */
            return (seq_t)0;
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Ensure that `comp` is a composite number. */
        while (is_prime_big(comp))
            comp++;

        /* Candidate prime = composite index + composite number. */
        seq_t cand = m + comp;
        if (is_prime_big(cand))
            cache[cache_len++] = cand;

        /* Advance to the next composite number and index. */
        m++;
        comp++;
    }

    return cache[n - 1];
}
