/*
 * A118625  Primes p such that the nearest integer to p * K is prime, where K
 *          is Catalan's constant.
 * https://oeis.org/A118625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that the nearest integer to p * K
 *            (Catalan's constant K ≈ 0.915965594177219015) is prime
 * Range:     u8 n <= 11, u16 n <= 546, u32 n <= 654, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has offset 1. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

#define K_NUM 915965594177219015ULL          /* round(K * 10^18) */
#define DEN    1000000000000000000ULL       /* 10^18  (must fit in seq_t) */
#define DEN_H  500000000000000000ULL        /* DEN / 2 */

static void grow_cache(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *arr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for a(0) */

    /* static cache of qualifying primes */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static seq_t   last_checked = 0;

    while (cache_len < (size_t)n) {
        /* find next prime */
        last_checked++;
        if (!is_prime_big(last_checked)) continue;

        seq_t p = last_checked;

        /* nearest integer to p * K:
           q = floor((p * K_NUM + DEN/2) / DEN)   (exact integer arithmetic) */
        seq_t q;
#if defined(__SIZEOF_INT128__)
        unsigned __int128 prod = (unsigned __int128)p * K_NUM + DEN_H;
        q = (seq_t)(prod / DEN);
#else
        /* Very small fallback using long double (should not be needed on
           common platforms where __int128 is available). */
        long double prod = (long double)p * ((long double)915965594177219015.0L / ((long double)1000000000000000000.0L));
        q = (seq_t)(prod + 0.5L);
#endif
        if (is_prime_big(q)) {
            grow_cache(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = p;
        }
    }
    return cache[n - 1];
}
