/*
 * A153321  Primes p such that abs(p^2 - 60) and p^2 + 60 are also primes.
 * https://oeis.org/A153321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that abs(p²-60) and p²+60 are both
 *            prime
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *cache      = NULL;   /* stored sequence values */
static size_t cache_len  = 0;      /* number of values generated */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t  next_candidate = 2;  /* next integer to test for primality */

/* Ensure the cache has at least 'needed' slots. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            /* Allocation failed – abort to avoid undefined behaviour */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Generate terms until cache_len reaches at least n. */
static void compute_next_terms(idx_t n)
{
    /* Loop until we have the required number of terms */
    while (cache_len < n) {
        /* Find the next candidate prime */
        for (idx_t p = next_candidate; ; ++p) {
            next_candidate = p + 1;          /* advance for next call */
            if (!is_prime(p)) continue;      /* skip composites */

            /* Compute p² in full precision */
            seq_t p2 = (seq_t)p * p;

            /* Compute |p²-60| and p²+60 */
            seq_t diff = (p2 > 60) ? (p2 - 60) : (60 - p2);
            seq_t sum  = p2 + 60;

            /* Test primality of the two values */
            if (is_prime_big(diff) && is_prime_big(sum)) {
                /* Store the prime p as a seq_t value */
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = (seq_t)p;
                break;                    /* proceed to next term */
            }
        }
    }
}

/* External linkage – required entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0 */
    compute_next_terms(n);
    return cache[n - 1];                  /* sequence is 1‑based */
}
