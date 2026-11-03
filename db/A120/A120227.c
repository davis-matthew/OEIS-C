/*
 * A120227  Numbers k such that 5+k and 5*k+1 are prime.
 * https://oeis.org/A120227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 556, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 5+k and 5*k+1 are prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

#ifdef __cplusplus
#include <vector>
#endif

/* Return the n-th element (n >= 1) of OEIS sequence A120227. */
seq_t a(idx_t n)
{
    /* Maximum index already computed. */
    static idx_t max_n = 0;

    /* In C++ we use std::vector<seq_t> for automatic resizing.
       In C we manage a dynamically allocated array manually. */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static seq_t next_k = 2;

    /* Ensure we have at least n terms stored. */
    while (cache.size() < (size_t)n) {
        /* Find the next k satisfying the primality conditions. */
        while (true) {
            seq_t k = next_k;
            next_k = k + 1;                 /* next candidate start point       */
            seq_t sum = k + ((seq_t)5);     /* 5 + k                            */
            seq_t expr = k * ((seq_t)5) + 1; /* 5*k + 1                          */
            if (is_prime_big(sum) && is_prime_big(expr)) {
                cache.push_back(k);         /* store the found term              */
                break;                       /* exit the inner search loop        */
            }
        }
    }
    return cache[(size_t)n - 1];
#else
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static seq_t next_k = 2;       /* next candidate value to test */

    /* Grow the cache as needed. */
    if (cache_len < (size_t)n) {
        /* Allocate space for at least n terms. */
        size_t needed = (size_t)n;
        if (cache_cap < needed) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Find new terms until we have n of them. */
        while (cache_len < (size_t)n) {
            /* Search for the next k satisfying the primality conditions. */
            for (;;) {
                seq_t k = next_k;
                next_k = k + 1;                 /* next starting point   */
                seq_t sum = k + ((seq_t)5);     /* 5 + k                */
                seq_t expr = k * ((seq_t)5) + 1;/* 5*k + 1              */
                if (is_prime_big(sum) && is_prime_big(expr)) {
                    cache[cache_len++] = k;   /* store the found term   */
                    break;                    /* exit the inner loop    */
                }
                /* continue searching */
            }
        }
    }

    return cache[(size_t)n - 1];
#endif
}
