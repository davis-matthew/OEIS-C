/*
 * A201739  Numbers n such that 90*n + 29 is prime.
 * https://oeis.org/A201739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 681, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9937 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers k such that 90*k + 29 is prime; a(n) returns the n‑th such k. */

#include <stdlib.h>
#include <stdbool.h>
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

/* Cache of already found k values. */
static seq_t *cache = NULL;   /* cache[i] == a(i+1)  for i < terms_found */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static idx_t terms_found = 0; /* number of terms stored in cache */
static idx_t next_k_to_check = 0; /* next k to examine in the search */

/* Ensure the cache is large enough for at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    if (!new_cache)
        abort(); /* memory allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n <= terms_found)
        return cache[n - 1];

    idx_t k = next_k_to_check;
    for (;;) {
        /* Compute 90*k + 29; cast k to seq_t to avoid intermediate overflow. */
        seq_t candidate = (seq_t)k * 90 + 29;
        if (is_prime_big(candidate)) {
            /* New term found: store k and update state. */
            ++terms_found;
            grow_cache(terms_found);
            cache[terms_found - 1] = (seq_t)k;

            if (terms_found == n) {
                next_k_to_check = k + 1;
                return cache[n - 1];
            }
        }
        ++k;
    }
}
