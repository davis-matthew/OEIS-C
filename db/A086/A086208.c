/*
 * A086208  Prime numbers p with the property that the sequence p-1*2, p-2*3,
 *          p-3*4, p-4*5, ..., consists only of primes and a final 1.
 * https://oeis.org/A086208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for primes p = k(k+1)+1 such that all p - i(i+1) (1 ≤ i < k) are prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;     /* cached terms */
static size_t cache_len = 0;    /* number of cached terms */
static size_t cache_cap = 0;    /* capacity of cache array */
static idx_t next_k = 1;        /* next k to test */

/* Ensure that the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t newcap = cache_cap ? cache_cap * 2 : 64;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }
}

/* Append a value to the cache. */
static void add_cache(seq_t val)
{
    ensure_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* Generate and cache the next term of the sequence. */
static void generate_next(void)
{
    for (;;) {
        idx_t k = next_k++;                     /* candidate k */
        seq_t p = (seq_t)k * ((seq_t)k + 1) + 1; /* p = k(k+1)+1 */

        if (!is_prime_big(p))
            continue;                           /* p must be prime */

        bool ok = true;
        for (idx_t i = 1; i < k; ++i) {
            seq_t sub = (seq_t)i * ((seq_t)i + 1);
            seq_t val = p - sub;
            if (!is_prime_big(val)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            add_cache(p);
            break;                               /* found next term */
        }
    }
}

/* Return the n-th term (a(0)=3). */
seq_t a(idx_t n)
{
    while (cache_len <= (size_t)n)
        generate_next();
    return cache[(size_t)n];
}
