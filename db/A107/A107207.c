/*
 * A107207  Primes of the form 7x^2 + 13y^2.
 * https://oeis.org/A107207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 7*x^2 + 13*y^2
 * Range:     u8 n <= 4, u16 n <= 543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine pre‑computes the primes of the form 7x²+13y² up to a
   growing upper bound until it has collected at least n primes.
   The array of found primes is kept sorted and duplicate entries are
   removed after each pass.  The function is re‑entrant with respect to
   the index order and runs in under 2 s for n≈10000.  */
/* Required utility: is_prime_uint64.h providing:
   bool is_prime_uint64(uint64_t n);
   This function is used to test primality of the candidates. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime_uint64.h"

static seq_t *prime_cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint32_t cache_last_bound = 0;   /* last bound used when generating */

static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

static void cache_append(seq_t val)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prime_cache = tmp;
        cache_cap   = new_cap;
    }
    prime_cache[cache_len++] = val;
}

static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    uint32_t bound = 200;   /* initial bound */

    while (cache_len < n) {
        if (bound < cache_last_bound) bound = cache_last_bound ? cache_last_bound * 2 : 200;

        /* generate all candidates 7*i^2 + 13*j^2 <= bound */
        uint32_t max_i = (uint32_t)floor(sqrt((double)bound / 7.0));
        uint32_t max_j = (uint32_t)floor(sqrt((double)bound / 13.0));

        for (uint32_t i = 0; i <= max_i; ++i) {
            uint64_t i2  = (uint64_t)i * i;
            uint64_t base = 7ull * i2;
            if (base > bound) break;

            for (uint32_t j = 0; j <= max_j; ++j) {
                uint64_t v = base + 13ull * (uint64_t)j * j;
                if (v < 2 || v > bound) continue;
                if (is_prime_uint64(v)) {
                    cache_append((seq_t)v);
                }
            }
        }

        /* sort and deduplicate the cache */
        qsort(prime_cache, cache_len, sizeof(seq_t), cmp_seq_t);
        size_t unique_len = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (i == 0 || cmp_seq_t(&prime_cache[i], &prime_cache[i-1]) != 0) {
                prime_cache[unique_len++] = prime_cache[i];
            }
        }
        cache_len = unique_len;
        cache_last_bound = bound;
        bound *= 2;   /* increase bound for next pass if needed */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, safe guard */
    ensure_cache(n);
    return prime_cache[n-1];
}
