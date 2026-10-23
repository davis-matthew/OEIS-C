/*
 * A141304  Primes of the form -2*x^2+6*x*y+3*y^2 (as well as of the form
 *          7*x^2+12*x*y+3*y^2).
 * https://oeis.org/A141304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 768, u32 n <= 769, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes p such that p = 3 or p ≡ 7 or 43 (mod 60). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static inline seq_t next_candidate(seq_t last)
{
    return last + (seq_t)2;  /* skip even numbers */
}

static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

static void extend_cache_to(idx_t n)
{
    if (n <= cache_len) return;

    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)3;     /* first term */
        cache_len = 1;
    }

    seq_t cand = cache[cache_len - 1];
    for (;;) {
        cand = next_candidate(cand);

        /* Only test primes that satisfy the congruence condition. */
        if (cand == (seq_t)3 ||
            (cand % (seq_t)60) == (seq_t)7 ||
            (cand % (seq_t)60) == (seq_t)43)
        {
            if (is_prime_big(cand)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = cand;
                if (cache_len >= n) break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0, but harmless */
    extend_cache_to(n);
    return cache[n - 1];
}
