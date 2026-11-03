/*
 * A140551  Primes p such that neither |p - 30| nor |p + 30| is prime.
 * https://oeis.org/A140551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes p such that neither |p-30| nor |p+30| is prime
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine builds the sequence on demand, memoising results in
   a static array that grows as needed.  It is thread‑unsafe but
   adequate for the intended use. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
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

static seq_t *cache = NULL;          /* stored terms of the sequence */
static size_t cache_len = 0;         /* number of terms currently stored */
static size_t cache_cap = 0;         /* current allocation size */

static seq_t next_candidate = 0;     /* next integer to test for primality */
static bool initialized = false;    /* true once a() has been called */

/* Append a new term to the cache, growing the allocation as needed. */
static void add_cache(seq_t val) {
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    } else if (cache_len == cache_cap) {
        cache_cap <<= 1;
        cache = mem_resize(seq_t, cache, cache_cap);
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term, 1‑based. */
seq_t a(idx_t n) {
    if (!initialized) {             /* first call – initialise state */
        initialized = true;
        next_candidate = 2;          /* start with the first prime */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;

        if (is_prime_big(cand)) {                /* cand is prime */
            seq_t diff1 = (cand > (seq_t)30) ?
                           cand - (seq_t)30 :
                           (seq_t)30 - cand;     /* |cand-30|  */
            seq_t diff2 = cand + (seq_t)30;      /* cand+30    */

            if (!is_prime_big(diff1) && !is_prime_big(diff2))
                add_cache(cand);
        }

        /* Advance to the next candidate – skip even numbers after 2. */
        if (cand <= 2)
            next_candidate = 3;
        else
            next_candidate = cand + 2;
    }

    return cache[n - 1];                   /* a(1) is the first element */
}
