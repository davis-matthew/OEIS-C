/*
 * A023228  Numbers k such that k and 8*k + 1 are both prime.
 * https://oeis.org/A023228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 174, u32 n <= 51, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k and 8*k+1 are both prime. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 2;          /* candidate to test next */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* below the offset – unused */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* find next k that satisfies the condition */
        for (;;) {
            seq_t k     = (seq_t)next_k;
            seq_t k8p1  = (seq_t)(8ULL * (uint64_t)next_k + 1ULL);

            if (is_prime_big(k) && is_prime_big(k8p1)) {
                break;           /* found a qualifying k */
            }
            ++next_k;
        }

        /* grow cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                abort();          /* allocation failed */
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* store the found term */
        cache[cache_len++] = (seq_t)next_k;
        ++next_k;             /* advance to the next candidate */
    }

    return cache[(size_t)n - 1];
}
