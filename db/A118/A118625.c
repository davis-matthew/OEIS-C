/*
 * A118625  Primes p such that the nearest integer to p * K is prime, where K
 *          is Catalan's constant.
 * https://oeis.org/A118625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that the nearest integer to p * K
 *            (Catalan's constant K ≈ 0.915965594177219015) is prime
 * Range:     u8 n <= 11, u16 n <= 686, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 974 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has offset 1. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

#define K_NUM 915965594177219015ULL          /* round(K * 10^18) */
#define DEN    1000000000000000000ULL       /* 10^18  (must fit in seq_t) */
#define DEN_H  500000000000000000ULL        /* DEN / 2 */

static void grow_cache(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, newcap);
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
