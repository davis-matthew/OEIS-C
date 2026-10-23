/*
 * A044578  Numbers k such that string 1,2 occurs in the base 8 representation
 *          of k but not of k+1.
 * https://oeis.org/A044578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044578: Numbers k such that the string 1,2 occurs in the
 * base‑8 representation of k but not in that of k+1.
 * Implementation uses a sieve-like incremental search with caching.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Internal helper: check whether the base‑8 representation of x contains
 * the consecutive digits 1 followed by 2.  We scan from least
 * significant digit to most significant digit, looking for a pair
 * 2 (current) and 1 (next more significant). */
static bool has12(seq_t x)
{
    /* Numbers with fewer than two base‑8 digits cannot contain "12". */
    if (x < (seq_t)8)
        return false;

    seq_t t = x;
    /* While at least two digits remain, examine the current (lsd)
     * and the next more significant digit. */
    while (t >= (seq_t)8) {
        seq_t cur  = t % (seq_t)8;          /* least significant digit */
        seq_t next = (t / (seq_t)8) % (seq_t)8; /* next digit */
        if (cur == (seq_t)2 && next == (seq_t)1)
            return true;
        t = t / (seq_t)8; /* shift right one digit */
    }
    return false;
}

/* Cache of computed terms.  Static so that repeated calls are fast. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t  cache_len = 0;      /* number of terms stored */
static size_t  cache_cap = 0;      /* current capacity of cache array */
static idx_t   last_k     = 0;      /* last k examined */

/* Return the n-th term of A044578 (1‑based index). */
seq_t a(idx_t n)
{
    /* Ensure the cache holds at least n terms. */
    while (cache_len < n) {
        /* Next candidate k to test. */
        idx_t k = last_k + 1;
        seq_t k_val = (seq_t)k;

        if (has12(k_val) && !has12(k_val + (seq_t)1)) {
            /* Append to cache (grow if necessary). */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache,
                                                    new_cap * sizeof(seq_t));
                if (!new_cache)
                    abort();        /* out of memory */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k_val;
        }
        last_k = k;
    }
    return cache[n - 1];
}
