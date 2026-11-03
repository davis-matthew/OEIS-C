/*
 * A232558  a(1)=5, q=a(n) is the smallest prime > a(n-1) such that q-2*n = p
 *          prime.
 * https://oeis.org/A232558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9793 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5; for n>1, a(n) = the smallest prime q > a(n‑1)
   such that q-2*n is prime.  The sequence is indexed from 1. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* Return the n‑th term of sequence A232558.  idx_t and seq_t are
   supplied by the build system. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms, 1‑based.  cache[0] is unused. */
    static seq_t *cache   = NULL;
    static size_t  alloc  = 0;   /* number of elements allocated */
    static idx_t   max_n  = 0;   /* largest index already computed */

    if (n == 0)          /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* Ensure we have space for indices 1 .. n.  Use seq_alloc_new_array
       because seq_t may be an arbitrary‑precision class. */
    if (alloc < (size_t)(n + 1)) {
        seq_t *new_cache = seq_alloc_new_array(n + 1);
        if (new_cache == NULL) {
            /* allocation failed – give up */
            return (seq_t)0;
        }
        /* copy existing contents */
        if (cache != NULL)
            mem_copy(new_cache, cache, alloc * sizeof(seq_t));
        /* free old array */
        if (cache != NULL)
            seq_free_array(cache);
        cache = new_cache;
        alloc = n + 1;
    }

    /* Initialise the first term if this is the first call. */
    if (max_n == 0) {
        cache[0] = (seq_t)0;   /* unused, but keeps indices – 1 based */
        cache[1] = (seq_t)5;   /* a(1)=5 */
        max_n   = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* previous term */
        seq_t prev = cache[k - 1];

        /* candidate prime > a(k-1) */
        uint64_t q = next_prime((uint64_t)prev);

        /* find the first q such that q-2*k is prime */
        for (;;) {
            seq_t p = (seq_t)q - (seq_t)(2 * k);   /* p = q - 2*k */
            if (is_prime_big(p)) {
                cache[k] = (seq_t)q;               /* store a(k) */
                break;
            }
            q = next_prime(q);                    /* try next prime */
        }
    }

    max_n = n;
    return cache[n];
}
