/*
 * A154679  Primes p such that the sum of digits of p+2 is a composite number.
 * https://oeis.org/A154679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 5327, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9635 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154679: Primes p such that the sum of digits of p+2 is a composite number.
   Code implements a direct sieve-like search, storing already found terms in a
   static cache.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Provided helper prototypes */
#include "../utils/digit_sum.h"
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
    /* Static cache holding the sequence values */
    static seq_t *cache = NULL;        /* array of terms */
    static size_t cache_len = 0;       /* number of terms stored */
    static size_t cache_cap = 0;       /* current capacity of cache */
    static uint64_t last_checked = 1;  /* last candidate prime examined */

    /* Ensure the cache is large enough to contain the nth term */
    while (cache_len < (size_t)n) {
        /* Find the next candidate prime after last_checked */
        uint64_t p = last_checked + 1;
        if (p <= 2)          p = 2;
        if (p > 2 && (p & 1) == 0) p++;      /* make odd if >2 */

        /* Search for the next qualifying prime */
        while (true) {
            /* Check primality of p */
            if (is_prime_big((seq_t)p)) {
                /* Compute sum of digits of p+2 */
                seq_t sum = digit_sum((seq_t)(p + 2));

                /* Requires a composite sum (sum > 1 and not prime) */
                if (sum > (seq_t)1 && !is_prime_big(sum)) {
                    /* Grow cache if needed */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                        if (!tmp) abort();            /* allocation failed */
                        cache = tmp;
                        cache_cap = new_cap;
                    }

                    /* Store the prime */
                    cache[cache_len++] = (seq_t)p;
                    last_checked = p;
                    break;                              /* next term found */
                }
            }

            /* Step to the next candidate, keeping it odd */
            p++;
            if (p > 2 && (p & 1) == 0) p++;
        }
    }

    return cache[(size_t)n - 1];
}
