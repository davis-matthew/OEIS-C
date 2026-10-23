/*
 * A032703  Numbers k such that k prefixed by '3' and followed by '1' is
 *          prime.
 * https://oeis.org/A032703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 977, u32 n <= 58, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the decimal concatenation 3k1 is prime.
   Computed by scanning k in increasing order and testing each candidate. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* stored k values that satisfy the condition */
static size_t cache_len = 0;     /* number of valid terms in cache */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t next_k = 1;         /* next k to test */

/* Return the nth term (1‑based) of A032703. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;               /* undefined for 0, fall back to 0 */
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        idx_t k = next_k++;
        seq_t k_seq = (seq_t)k;

        /* Number of decimal digits in k. */
        int d = digit_len(k_seq);

        /* Compute 10^(d+1) in a 64‑bit intermediate to avoid overflow of seq_t. */
        uint64_t pow10 = 1;
        for (uint64_t i = 0; i < (uint64_t)(d + 1); ++i) {
            pow10 *= 10ULL;
        }

        /* Construct N = 3 * 10^(d+1) + k * 10 + 1. */
        uint64_t N = 3ULL * pow10 + (uint64_t)k * 10ULL + 1ULL;
        seq_t term = (seq_t)N;

        /* Test primality of the constructed number. */
        if (is_prime_big(term)) {
            /* Expand the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    abort(); /* allocation failed */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k_seq;
        }
    }

    return cache[n - 1];
}
