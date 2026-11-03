/*
 * A158075  Prime numbers p such that (the largest digit of p) - (the smallest
 *          digit of p) is prime.
 * https://oeis.org/A158075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = nth prime p such that (max digit of p)−(min digit of p)
 *            is prime (2,3,5,7)
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; a(0) is undefined, return 0 as a placeholder */
        return (seq_t)0;

    /* Static cache of qualifying primes */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;  /* number of elements currently stored */
    static size_t cache_cap  = 0;  /* current allocation size */
    static idx_t next_candidate = 2;   /* next integer to test for primality */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        /* Test the current candidate */
        seq_t p = (seq_t)next_candidate;
        if (is_prime_big(p))
        {
            /* Extract decimal digits of p */
            seq_t tmp = p;
            uint8_t min_d = 9, max_d = 0;
            while (tmp > 0)
            {
                uint8_t d = (uint8_t)(tmp % 10);
                if (d < min_d) min_d = d;
                if (d > max_d) max_d = d;
                tmp /= 10;
            }
            uint8_t diff = max_d - min_d;
            if (diff == 2 || diff == 3 || diff == 5 || diff == 7)
            {
                /* Grow capacity if needed */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                    if (!new_cache)   /* allocation failed */
                        return (seq_t)0;
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = p;
            }
        }

        /* Advance to the next candidate (skip even numbers > 2) */
        next_candidate++;
        if (next_candidate > 2 && (next_candidate & 1) == 0)
            next_candidate++;
    }

    return cache[n - 1];
}
