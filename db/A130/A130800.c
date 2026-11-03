/*
 * A130800  Numbers k such that both 2k+1 and 3k+1 are primes.
 * https://oeis.org/A130800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 583, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 990 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th integer k (1‑based) such that both 2k+1
   and 3k+1 are prime numbers. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* Static state to allow out‑of‑order calls */
    static seq_t *cache      = NULL;   /* array of found k's          */
    static size_t cache_cap  = 0;      /* capacity of cache           */
    static idx_t  cache_cnt  = 0;      /* number of terms stored      */
    static seq_t  next_k     = 1;      /* next candidate k to test    */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return 0;  /* allocation failure */
    }

    /* Produce terms until we have at least n of them */
    while (cache_cnt < n) {
        /* Check primality of 2k+1 and 3k+1 */
        seq_t two_k_plus_one = (seq_t)2 * next_k + (seq_t)1;
        seq_t three_k_plus_one = (seq_t)3 * next_k + (seq_t)1;

        if (is_prime_big(two_k_plus_one) && is_prime_big(three_k_plus_one)) {
            /* Grow cache if necessary */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) return 0;  /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the found k */
            cache[cache_cnt++] = next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
