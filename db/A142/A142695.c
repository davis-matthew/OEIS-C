/*
 * A142695  Primes congruent to 49 mod 57.
 * https://oeis.org/A142695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 49 (mod 57)
 * Range:     u8 n <= 1, u16 n <= 171, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 927 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *prime_cache = NULL;   /* dynamically grown cache of found primes */
static size_t cache_cap   = 0;       /* current capacity of the cache        */
static size_t cache_len   = 0;       /* number of primes stored so far       */
static idx_t   k_next     = 0;       /* next k to try for candidate 57*k+49  */

/* Grow the cache so that it can hold at least 'needed' primes */
static void ensure_cache(idx_t needed)
{
    while (cache_len < (size_t)needed) {
        seq_t candidate = (seq_t)57 * (seq_t)k_next + (seq_t)49;
        if (is_prime_big(candidate)) {
            if (cache_len >= cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, prime_cache, newcap);
                if (!tmp) exit(EXIT_FAILURE);  /* OOM – should never happen in tests */
                prime_cache = tmp;
                cache_cap   = newcap;
            }
            prime_cache[cache_len++] = candidate;
        }
        ++k_next;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n<1, return 0 */
    ensure_cache(n);
    return prime_cache[n - 1];
}
