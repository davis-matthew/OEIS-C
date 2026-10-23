/*
 * A155847  Primes of the form 2^p-p*q where p is prime,q=7.
 * https://oeis.org/A155847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 4, i64 n <= 4, i128 n <= 4,
 *            big any n
 * Helpers:   is_prime, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155847: primes of the form 2^p – 7p where p is prime.
   The first few terms are a(1) = -13, a(2) = -3, a(3) = 79, a(4) = 8101. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t x) */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n) */
#include "../utils/pow2.h"             /* seq_t pow2(idx_t exp) */

static seq_t *seq_cache = NULL;   /* dynamically grown array of terms */
static size_t cache_len = 0;       /* number of terms currently stored */
static size_t cache_cap = 0;       /* allocated capacity of the array */

/* Find the next prime after the given number.
   Uses a simple incremental test; p must be > 1. */
static idx_t next_prime(idx_t start)
{
    if (start <= 2) return 2;
    if (start == 3) return 3;
    if (start % 2 == 0) start++;   /* make odd */
    for (idx_t n = start; ; n += 2) {
        if (is_prime(n)) return n;
    }
}

/* Append a value to the cache, resizing if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t newcap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* allocation failed */
        seq_cache = tmp;
        cache_cap = newcap;
    }
    seq_cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0, but safe */
    /* Ensure cache contains at least n terms. */
    if (cache_len < n) {
        idx_t last_checked = (cache_len == 0) ? 1 : 0;  /* start searching from 2 */
        /* Find the largest p already processed for cache_len terms. */
        for (idx_t i = 1; i < cache_len; ++i) {
            /* _since we can't store p, we recompute it lazily_,
               but we don't need p here; we use incremental primes. */
        }
        /* Keep searching next primes and adding qualifying terms. */
        static idx_t last_prime = 1;   /* last prime examined when filling cache */
        while (cache_len < n) {
            last_prime = next_prime(last_prime + 1);
            /* Compute v = 2^p – 7p */
            seq_t v = pow2(last_prime) - ((seq_t)last_prime * 7);
            /* Test primality of |v|. Ignore |v| <= 1. */
            seq_t absv = (v < 0) ? -v : v;
            if (absv > 1 && is_prime_big(absv))
                cache_append(v);
        }
    }
    return seq_cache[n - 1];
}
