/*
 * A073640  a(1) = 2; a(n) = smallest prime greater than the previous term
 *          such that concatenation of two successive terms is a prime.
 * https://oeis.org/A073640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=2; a(n) = smallest prime > a(n-1) such that concatenation
 *            of a(n-1) and that prime is also prime
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   concat_parts, is_prime_big, to_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/concat_parts.h"
#include "../utils/to_idx_t.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache[1] = a(1), etc. */
    static idx_t cache_len = 0;     /* largest index computed so far */

    if (n == 0) {
        /* The sequence is defined for n >= 1; behavior for n = 0 is irrelevant. */
        return 0;
    }

    /* Ensure the cache is large enough */
    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) exit(1);          /* Out of memory */
        cache = tmp;
    }

    /* Initialise the sequence if it hasn't been started yet */
    if (cache_len == 0) {
        cache[1] = 2;
        cache_len = 1;
    }

    /* Compute missing elements up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cand = prev + 1;      /* start searching from the next integer */

        /* Search for the next prime that also satisfies the concatenation test */
        while (true) {
            if (!is_prime_big(cand)) {
                cand++;               /* try the next integer */
                continue;
            }

            /* Concatenate prev and cand; the helper expects 64‑bit inputs */
            seq_t concat = concat_parts(to_idx_t(prev), to_idx_t(cand));

            if (is_prime_big(concat)) {
                cache[i] = cand;      /* Found the desired term */
                break;
            }
            cand++;                   /* try the next integer */
        }
    }

    cache_len = n;
    return cache[n];
}
