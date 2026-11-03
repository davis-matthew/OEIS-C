/*
 * A138628  Positive integers k such that 17*k-7 is prime.
 * https://oeis.org/A138628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 525, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9745 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138628: a(n) = smallest positive integer k such that 17*k-7 is prime. */
/* The function caches found values and grows as needed. */
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
    /* Cache of found k values (cache[0] = a(1)). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    /* Next k to test. */
    static idx_t next_k = 1;

    /* Allocate initial cache if it does not exist yet. */
    if (cache == NULL)
    {
        cache_cap = 1024;
        cache = mem_new(seq_t, cache_cap);
        if (!cache)
            return (seq_t)0;          /* allocation failed – return 0 as a sentinel */
    }

    /* Grow cache until we have the requested number of terms. */
    while (cache_size < n)
    {
        /* Compute candidate value 17*k-7 in an integer type able to hold the result. */
        unsigned __int128 cand128 = (unsigned __int128)17 * next_k - 7;
        seq_t cand = (seq_t)cand128;        /* cast to seq_t before primality test */

        if (is_prime_big(cand))
        {
            /* Ensure capacity. */
            if (cache_size == cache_cap)
            {
                cache_cap *= 2;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp)
                {
                    mem_free(cache);
                    cache = NULL;
                    return (seq_t)0;      /* allocation failed */
                }
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)next_k;
        }
        next_k++;
    }

    return cache[n - 1];
}
