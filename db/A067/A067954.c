/*
 * A067954  Primes related to the nondecreasing subsequence of A007605 (sums
 *          of digits of primes).
 * https://oeis.org/A067954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=2; a(n+1) is the smallest prime with digit sum >= sum of
 *            digits of a(n)
 * Range:     u8 n <= 14, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

static seq_t *cache = NULL;          /* cache of computed values */
static size_t cache_cap = 0;         /* capacity of cache */
static idx_t cache_len = 0;          /* number of values stored in cache */
static seq_t last_prime;             /* last prime computed (a(n)) */
static seq_t last_sum;               /* sum of digits of last_prime */

/* Ensure that cache contains values up to index n (1‑based) */
static void cache_ensure(idx_t n)
{
    while (cache_len < n) {
        if (cache_len == 0) {            /* initialize first term */
            last_prime = 2;
            last_sum   = 2;
            if (cache_cap == 0) {
                cache_cap = 32;
                cache = mem_new(seq_t, cache_cap);
            }
            cache[0] = last_prime;
            ++cache_len;
            continue;
        }

        /* Find next prime with sum of digits >= last_sum */
        seq_t candidate = last_prime + 1;
        while (true) {
            if (is_prime_big(candidate)) {
                seq_t s = digit_sum(candidate);
                if (s >= last_sum) {
                    last_prime = candidate;
                    last_sum   = s;
                    break;
                }
            }
            ++candidate;
        }

        /* Grow cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 32;
            cache = mem_resize(seq_t, cache, new_cap);
            cache_cap = new_cap;
        }

        cache[cache_len++] = last_prime;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 (behavior irrelevant) */
    cache_ensure(n);
    return cache[n - 1];
}
