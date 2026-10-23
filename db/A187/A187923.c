/*
 * A187923  Numbers n of the form 4*k+3 such that 2^(m-1) == 1 (mod m) where m
 *          = (2*n-1)*n.
 * https://oeis.org/A187923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187923: numbers n ≡ 3 (mod 4) such that 2^(m-1) ≡ 1 (mod m)
   where m = n*(2*n-1).  The function returns the n‑th such number. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Cache the computed terms to avoid recomputation on subsequent calls. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* number of terms already stored */
    static idx_t next_candidate = 3;   /* next n ≡ 3 (mod 4) to test */
    static int initialized = 0;

    /* Initialise cache on first use. */
    if (!initialized) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_candidate = 3;
        initialized = 1;
    }

    /* Grow cache if necessary. */
    if (cache_len < n && cache_cap < cache_len + 1) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        idx_t cand = next_candidate;                /* candidate n */
        seq_t n_seq = (seq_t)cand;
        seq_t m     = ((seq_t)2 * n_seq - (seq_t)1) * n_seq;  /* m = n*(2n-1) */

        /* Exponent m-1 must fit in uint64_t because mod_pow's exp is uint64_t. */
        uint64_t exp = (uint64_t)(m - (seq_t)1);

        seq_t powmod = mod_pow((seq_t)2, exp, m);   /* compute 2^(m-1) mod m */

        if (powmod == (seq_t)1) {                  /* condition satisfied */
            cache[cache_len++] = (seq_t)cand;       /* store term */
        }
        /* Advance to the next candidate of the form 4k+3. */
        next_candidate += 4;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
