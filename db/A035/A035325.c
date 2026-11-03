/*
 * A035325  Primes of the form 2^k + k^2 + 1.
 * https://oeis.org/A035325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A035325: primes of the form 2^k + k^2 + 1.
   k is iterated over the multiples of 6 (A100357).  The function
   memoises results so repeated calls are fast.  Uses the helper
   is_prime_big() for primality testing. */
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
    /* Memoisation: cache of found terms. */
    static seq_t *cache = NULL;    /* array of primes found */
    static size_t cap = 0;          /* capacity of cache */
    static size_t sz  = 0;          /* number of filled entries */
    static idx_t  last_k = 0;       /* last k processed (multiple of 6) */
    static seq_t power2 = 1;        /* 2^last_k */

    /* Initialise cache on first call. */
    if (!cache) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        last_k = 0;        /* start with k = 0 */
        power2 = 1;        /* 2^0            */
    }

    /* If the desired term is already computed, return it. */
    if (sz >= n) return cache[n - 1];

    /* Generate primes until we have at least n terms. */
    while (sz < n) {
        /* Current k as seq_t for arithmetic. */
        seq_t k   = (seq_t)last_k;
        seq_t val = power2;       /* 2^k          */
        val += k * k;             /* + k^2        */
        val += 1;                 /* + 1          */

        /* Test primality. */
        if (is_prime_big(val)) {
            /* Grow cache if necessary. */
            if (sz >= cap) {
                cap *= 2;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[sz++] = val;
        }

        /* Advance to the next multiple of 6. */
        last_k += 6;
        /* Multiply power2 by 64 (2^6) for the new k. */
        power2 *= (seq_t)64;
    }

    return cache[n - 1];
}
