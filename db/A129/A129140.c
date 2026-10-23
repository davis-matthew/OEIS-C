/*
 * A129140  a(0)=a(1)=1. a(n) = the n-th integer from among those positive
 *          integers coprime to (a(n-1)*a(n-2)).
 * https://oeis.org/A129140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=1; for n≥2, a(n) = the n‑th positive integer coprime
 *            to a(n‑1)*a(n‑2)
 * Range:     u8 n <= 10, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Requirements:  see problem statement */

#include <stdlib.h>
#include "../utils/seq_gcd.h"   /* seq_gcd(seq_t a, seq_t b) */

static seq_t find_nth_coprime(seq_t D, size_t rank)
{
    seq_t x = (seq_t)1;
    size_t cnt = 0;

    while (1) {
        if (seq_gcd(x, D) == (seq_t)1) {
            ++cnt;
            if (cnt == rank)
                return x;
        }
        ++x;
    }
}

/* Sequence cache */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            exit(1);
        }
    }

    /* grow cache up to index n */
    while (cache_len <= (size_t)n) {
        seq_t val;
        if (cache_len == 0 || cache_len == 1) {
            val = (seq_t)1;
        } else {
            seq_t prod = cache[cache_len - 1] * cache[cache_len - 2];
            val = find_nth_coprime(prod, (size_t)cache_len);
        }

        /* extend cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) {
                exit(1);
            }
            cache = tmp;
        }

        cache[cache_len++] = val;
    }

    return cache[(size_t)n];
}
