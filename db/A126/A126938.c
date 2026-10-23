/*
 * A126938  a(1) = 3, a(n) = the smallest prime p > a(n-1) such that
 *          (a(n-1)+p)/2 is prime.
 * https://oeis.org/A126938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3; for n > 1, a(n) is the smallest prime p > a(n‑1) such
 *            that (a(n‑1)+p)/2 is also prime
 * Range:     u8 n <= 10, u16 n <= 403, u32 n <= 287, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* current capacity of cache     */
    static idx_t  computed = 0;   /* number of terms already filled  */

    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, but harmless */

    /* Return a previously computed term if possible */
    if (n <= computed)
        return cache[n - 1];

    /* Allocate or expand cache to at least one element */
    if (cap < 1) {
        cache = (seq_t*)realloc(cache, sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */
        cap = 1;
    }

    /* Store the first term if not already stored */
    if (computed == 0) {
        cache[0] = (seq_t)3;   /* a(1) = 3 */
        computed = 1;
    }

    /* Ensure capacity for the required number of terms */
    if (n > cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms from computed+1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];          /* a(i-1) */

        /* Start searching for the next prime bigger than prev */
        seq_t cand = prev + (seq_t)1;
        if ((cand & (seq_t)1) == (seq_t)0)  /* make cand odd if even */
            cand += (seq_t)1;

        /* Find the smallest prime cand such that (prev + cand)/2 is prime */
        for (;;) {
            if (!is_prime_big(cand))
                cand += (seq_t)2;
            else {
                seq_t mid = (prev + cand) / (seq_t)2;
                if (is_prime_big(mid)) {
                    cache[i - 1] = cand;   /* a(i) = cand */
                    break;
                }
                cand += (seq_t)2;
            }
        }
    }

    computed = n;
    return cache[n - 1];
}
