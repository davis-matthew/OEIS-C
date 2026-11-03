/*
 * A158458  Numbers k such that k + bigomega(k) is prime.
 * https://oeis.org/A158458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that k + bigomega(k) is prime (OEIS A158458)
 * Range:     u8 n <= 52, u16 n <= 6844, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8570 (2026-11-03)
 * Helpers:   bigomega, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bigomega.h"
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

static seq_t *cache = NULL;          /* stored sequence values */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t  processed_k = 1;       /* next integer to test (start with 2) */

/* Ensure that cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        ++processed_k;                             /* candidate integer k */
        seq_t omega = bigomega(processed_k);        /* Ω(k) */
        seq_t cand  = (seq_t)processed_k + omega;   /* k + Ω(k) */

        if (is_prime_big(cand)) {                  /* check primality */
            if (cache_len == cache_cap) {           /* grow cache if needed */
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)processed_k;/* store term */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but safe guard */
    ensure_cache(n);
    return cache[n - 1];
}
