/*
 * A215528  Slowest increasing sequence of alternate-parity integers m such
 *          that 2m^2+1 is prime.
 * https://oeis.org/A215528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 983 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A215528 – slowest increasing sequence of alternate‑parity integers
   m such that 2*m^2+1 is prime.  Index a(1)=1.  The implementation
   memoises previously computed terms and satisfies any ordering of calls. */

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
    /* Static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    if (n == 0)             /* undefined, but guard against accidental use */
        return (seq_t)0;
    if (n <= cache_len)     /* already computed */
        return cache[n - 1];

    /* Ensure the first term a(1)=1 exists */
    if (cache_len == 0) {
        if (!cache_cap) {
            cache_cap = 1;
            cache = mem_new(seq_t, cache_cap);
        }
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Generate terms until we have the n‑th term */
    while (cache_len < n) {
        seq_t prev = cache[cache_len - 1];
        seq_t cand = prev + (seq_t)1;   /* opposite parity to prev */

        /* Search for the next valid cand */
        while (1) {
            seq_t val = cand * cand;   /* cand^2 */
            val = val * (seq_t)2;      /* 2*cand^2 */
            val = val + (seq_t)1;      /* 2*cand^2 + 1 */

            if (is_prime_big(val))
                break;                /* cand satisfies the condition */

            cand += (seq_t)2;          /* keep alternate parity */
        }

        /* Grow cache if necessary */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)                 /* allocation failure */
                exit(EXIT_FAILURE);
            cache     = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len] = cand;
        cache_len++;
    }

    return cache[n - 1];
}
