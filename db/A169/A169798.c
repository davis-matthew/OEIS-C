/*
 * A169798  a(n) = smallest prime number > a(n-1) that contains the n-th
 *          composite number as a substring.
 * https://oeis.org/A169798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime > a(n-1) that contains the n-th composite
 *            number as a substring
 * Range:     u8 n <= 10, u16 n <= 388, u32 n <= 531, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_to_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires utilities: is_prime_big. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime_big.h"
#include "../utils/seq_to_decimal.h"

seq_t a(idx_t n)
{
    /* Cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* State used when extending the cache */
    static seq_t last_prime = 0;   /* a(last index) */
    static seq_t last_comp  = 3;   /* 3 < 4 (first composite) */

    /* Grow cache if necessary */
    if ((idx_t)cache_len < n) {
        size_t needed = (size_t)n;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate terms until we have at least n */
        while (cache_len < n) {
            /* Find next composite number greater than last_comp */
            seq_t comp = last_comp + 1;
            while (is_prime_big(comp)) comp++;      /* skip primes */
            last_comp = comp;

            /* Convert composite to string once */
            char comp_buf[64];
            seq_to_decimal(comp, comp_buf);

            /* Search for next prime > last_prime that contains comp */
            seq_t p = last_prime + 1;
            for (;;) {
                if (is_prime_big(p)) {
                    char prime_buf[4096];
                    seq_to_decimal(p, prime_buf);
                    if (strstr(prime_buf, comp_buf) != NULL) break;
                }
                p++;                               /* try next integer */
            }

            /* Store the found prime and update state */
            cache[cache_len++] = p;
            last_prime = p;
        }
    }

    /* Return the requested term (indices are 1‑based) */
    return cache[n - 1];
}
