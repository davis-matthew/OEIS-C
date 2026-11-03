/*
 * A136241  Numbers n among A006093 such that n^2 + n - 1 is prime.
 * https://oeis.org/A136241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A091567(n) - 1 (Numbers n such that n+1 is prime and n^2
 *            + n - 1 is prime.) The code searches through the integers, tests
 *            each prime p for the additional primality condition p^2 - p - 1,
 *            and records n = p-1
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 930 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
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

static seq_t *cache = NULL;      /* array of sequence terms n */
static size_t cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t next_int = 1;       /* next integer to test (starts at 2) */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();   /* unrecoverable error */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */
    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        ++next_int;                /* try integer next_int as candidate p */
        seq_t p = (seq_t)next_int; /* p must be prime (p = n+1) */
        if (!is_prime_big(p)) continue;  /* p not prime, skip */
        seq_t val = p * p - p - (seq_t)1; /* compute p^2 - p - 1 */
        if (!is_prime_big(val)) continue; /* extra condition fails */
        /* p satisfies both conditions -> add n = p-1 to cache */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = p - (seq_t)1;
    }
    return cache[n - 1];           /* 1‑based indexing */
}
