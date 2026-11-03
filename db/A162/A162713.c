/*
 * A162713  Primes of the form 3^p-2^p-2 generated from primes p.
 * https://oeis.org/A162713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 3^p – 2^p – 2 where p is prime.
   The sequence a(n) lists these primes in the order of increasing p. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities available in the build */
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* ------------------------------------------------------------------ */
/* Main sequence generator                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Static state persisted across calls */
    static bool initialized = false;
    static idx_t cur_exp = 0;            /* current exponent (p)            */
    static seq_t pow3 = (seq_t)1;         /* 3^cur_exp                       */
    static seq_t pow2 = (seq_t)1;         /* 2^cur_exp                       */
    static seq_t *cache = NULL;          /* cache of found primes           */
    static size_t cache_len = 0;         /* number of terms cached          */
    static size_t cache_cap = 0;         /* current cache capacity          */

    if (!initialized) {
        /* Nothing special to do on first call – state already zeroed. */
        initialized = true;
    }

    /* Generate further terms until we have at least n of them. */
    while (cache_len < n) {
        /* advance to next exponent */
        cur_exp++;
        pow3 = seq_mul(pow3, (seq_t)3);
        pow2 = seq_mul(pow2, (seq_t)2);

        /* If p is prime, test the candidate value */
        if (is_prime(cur_exp)) {
            seq_t val = pow3;
            val -= pow2;          /* val = 3^p - 2^p */
            val -= (seq_t)2;      /* val = 3^p - 2^p - 2 */
            if (is_prime_big(val)) {
                /* Ensure capacity in the cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) { /* allocation failure */
                        exit(EXIT_FAILURE);
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
            }
        }
    }

    return cache[n - 1];
}
