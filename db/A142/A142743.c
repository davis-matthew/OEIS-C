/*
 * A142743  Primes congruent to 16 mod 59.
 * https://oeis.org/A142743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p mod 59 == 16
 * Range:     u8 n <= 1, u16 n <= 112, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 971 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_count = 0;
    static seq_t last_prime = 0;

    if (n == 0) /* sequence is 1‑based; undefined for 0, return 0 */
        return 0;

    /* Return cached value if already known */
    if (cache_count >= (size_t)n)
        return cache[n - 1];

    /* Starting candidate for the next prime of the required form */
    seq_t cand = (cache_count == 0) ? 16 : last_prime + 59;

    /* Generate primes until we have at least n of them */
    while (cache_count < (size_t)n)
    {
        /* Find next prime that is 16 mod 59 */
        while (!is_prime_big(cand))
            cand += 59;

        /* Ensure enough space in the cache */
        if (cache_count >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();          /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the found prime */
        cache[cache_count++] = cand;
        last_prime = cand;
        cand += 59; /* next candidate to test */
    }

    return cache[n - 1];
}
