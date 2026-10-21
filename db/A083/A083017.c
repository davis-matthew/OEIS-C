/*
 * A083017  Primes whose 10's complement is a palindrome.
 * https://oeis.org/A083017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_palseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime p such that 10^d – p is a
   decimal palindrome (OEIS A083017, offset 1).  The implementation
   builds the sequence on demand by repeatedly sieve‑ing larger ranges
   and caching the qualifying primes.  It works for all supported
   seq_t sizes. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_palseq.h"

/* Cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Highest bound already sieved */
static idx_t last_sieve = 0;

/* Append a term to the cache, expanding it if necessary */
static void cache_append(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* Sieve up to 'lim' and collect qualifying primes in (last_sieve, lim] */
static void sieve_collect(idx_t lim)
{
    /* Allocate sieve bitmap */
    size_t sz = (size_t)lim + 1;
    uint8_t *mark = (uint8_t *)calloc(sz, 1);
    if (!mark) exit(1);

    /* Mark 0 and 1 as composite */
    if (sz > 0) mark[0] = 1;
    if (sz > 1) mark[1] = 1;

    /* Eratosthenes up to sqrt(lim) */
    idx_t sq = 0;
    while ((idx_t)((unsigned long long)sq * sq) <= lim) ++sq;
    for (idx_t p = 2; p < sq; ++p) {
        if (!mark[p]) {
            for (idx_t j = p * p; j <= lim; j += p) mark[j] = 1;
        }
    }

    /* Start testing from max(2, last_sieve+1) */
    idx_t start = last_sieve + 1;
    if (start < 2) start = 2;
    for (idx_t i = start; i <= lim; ++i) {
        if (!mark[i]) {
            seq_t p = (seq_t)i;

            /* Number of decimal digits of p */
            int d = 0;
            seq_t tmp = p;
            while (tmp > 0) {
                tmp /= 10;
                ++d;
            }

            /* 10^d */
            seq_t pow10 = 1;
            for (int k = 0; k < d; ++k) pow10 *= 10;

            /* Complement */
            seq_t comp = pow10 - p;
            if (is_palseq(comp)) {
                cache_append(p);
            }
        }
    }

    free(mark);
    last_sieve = lim;
}

/* Ensure that at least n terms are present in the cache */
static void ensure_n(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* Double the sieve range each time (at least 1000) */
        idx_t new_lim = last_sieve ? last_sieve * 2 : 1000;
        if (new_lim < 1000) new_lim = 1000;
        if (new_lim <= last_sieve) new_lim = last_sieve + 1000;
        sieve_collect(new_lim);
    }
}

/* --------------------------------------------------------------- */
/* Sequence entry function requested by the problem statement     */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: offset 1 */
    ensure_n(n);
    return cache[n - 1];
}
