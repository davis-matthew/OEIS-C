/*
 * A145096  Primes of the form 4*k^2 - 38*k + 131.
 * https://oeis.org/A145096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime value of the quadratic 4*k^2 - 38*k + 131
 *            (k starts at 0)
 * Range:     u8 n <= 13, u16 n <= 98, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 976 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Static cache to store already found primes of the form 4*k^2 - 38*k + 131. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of primes stored in cache     */
static idx_t cache_cap  = 0;   /* current capacity of cache array       */
static idx_t next_k     = 0;   /* the next k value to test for primes   */

static void ensure_cache_capacity(idx_t required)
{
    if (required <= cache_cap)
        return;
    /* Grow cache: double size until we exceed required capacity */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < required)
        new_cap *= 2;
    seq_t *tmp = mem_new(seq_t, new_cap);
    if (!tmp) {
        /* In a real project we would handle out‑of‑memory here.  */
        abort();
    }
    for (idx_t i = 0; i < cache_size; ++i)
        tmp[i] = cache[i];
    mem_free(cache);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return the result if it is already cached. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Generate primes until we have at least n of them. */
    for (idx_t k = next_k; ; ++k) {
        seq_t kk = (seq_t)k;
        seq_t val = (seq_t)4 * kk * kk - (seq_t)38 * kk + (seq_t)131;
        if (is_prime_big(val)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = val;
            if (cache_size >= n) {
                next_k = k + 1;          /* next search starts after current k */
                break;
            }
        }
    }
    return cache[n - 1];
}
