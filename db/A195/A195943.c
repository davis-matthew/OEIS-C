/*
 * A195943  Zeroless prime powers: Intersection of A000961 and A052382.
 * https://oeis.org/A195943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 4926, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A195943: Zeroless prime powers – intersection of prime powers and
   integers with no zero digit.
   The function generates numbers in increasing order, checking for
   the two requirements, and caches the results for arbitrary order
   queries. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

/* Check whether a decimal representation contains the digit 0. */
static bool has_zero_digit(uint64_t x)
{
    while (x > 0) {
        if ((x % 10ULL) == 0ULL)
            return true;
        x /= 10ULL;
    }
    return false;
}

/* Determine whether a 64‑bit integer is a prime power.
   (1 is included as a prime power by convention.) */
static bool is_prime_power(uint64_t x)
{
    if (x <= 1)
        return x == 1;            /* 1 is the first term */

    uint64_t p = largest_prime_divisor(x);

    if (p == x)                 /* prime */
        return true;

    uint64_t t = x;
    while (t % p == 0)
        t /= p;
    return t == 1;              /* all prime factors equal p */
}

/* ---------- Cached memoisation ---------- */
static seq_t  *cache  = NULL;   /* array of computed terms          */
static size_t  cache_len = 0;   /* number of terms stored           */
static size_t  cache_cap = 0;   /* current allocation capacity      */
static uint64_t next_val = 2;   /* next integer to test (64‑bit)    */

/* Return the nth term of the sequence (1‑based).  The function is
   correct for any n >= 1 and can be called in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but protects against bad input */

    if (cache_len == 0) {
        /* initialise the cache with the first term 1 */
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();     /* fatal if we cannot allocate */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t m = next_val++;
        if (m == 0) break;       /* overflow – should not happen for required n */

        if (has_zero_digit(m))
            continue;            /* skip numbers containing zero */

        if (is_prime_power(m)) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap <<= 1; /* double capacity */
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort(); /* fatal if memory cannot be expanded */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)m; /* store term */
        }
    }

    return cache[(size_t)n - 1];
}
