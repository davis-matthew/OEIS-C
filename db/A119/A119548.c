/*
 * A119548  10-gonal numbers which are divisible by the sum of their digits.
 * https://oeis.org/A119548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 5395, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_buf, digit_sum, free_buf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A119548
 *  a(n)  = the n‑th 10‑gonal number that is divisible by the sum of its
 *          decimal digits.
 *  10‑gonal number for index m (m ≥ 1):
 *          P(10,m) = m * (4*m - 3)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/alloc_buf.h"
#include "../utils/free_buf.h"

/* Swap the type of these declarations depending on the language. */
#ifdef __cplusplus

#else

#endif

/* Static state: a cache of found terms, its size and how far we have
 * checked the 10‑gonal indices.  These variables retain values between
 * calls so the function works for arbitrary call order. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t  next_m   = 1;      /* next 10‑gonal index to test */

/* Ensure that the cache is large enough; enlarge by doubling if necessary. */
static void ensure_cache_capacity(void)
{
    if (cache_len + 1 <= cache_cap) return;          /* space already available */

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;    /* start with 8 entries */
    seq_t *new_cache = alloc_buf(new_cap);

    /* Copy existing terms.  For a class type this invokes the copy ctor. */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

    /* Release old buffer. */
    if (cache) free_buf(cache);

    cache        = new_cache;
    cache_cap    = new_cap;
}

/* Store a new term in the cache. */
static void cache_push(seq_t val)
{
    ensure_cache_capacity();
    cache[cache_len++] = val;
}

/* Return the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t m   = (seq_t)next_m;          /* convert index to seq_t type   */
        seq_t val = m * ((seq_t)4 * m - (seq_t)3);   /* 10‑gonal formula */

        seq_t sum = digit_sum(val);        /* sum of decimal digits        */
        if (sum != 0 && (val % sum) == 0) {
            cache_push(val);                /* record qualifying term        */
        }
        ++next_m;
    }
    return cache[(size_t)n - 1];
}
