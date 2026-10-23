/*
 * A046827  Numbers k such that k^2 contains all the digits of k with the same
 *          or higher multiplicity.
 * https://oeis.org/A046827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046827: k such that k² contains all the digits of k with
   at least the same multiplicity.  The term a(n) is the n‑th such
   integer (a(1)=0).  The algorithm enumerates k in increasing order
   and tests the property directly. */
/*--------------------------------------------------------------*/
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"

static seq_t *cache          = NULL;   /* stored terms              */
static size_t cache_len      = 0;      /* number of terms cached    */
static size_t cache_cap      = 0;      /* current cache capacity    */
static seq_t  last_k         = 0;      /* next candidate to test    */
static bool    initialized   = false; /* cache initialised?        */

/* Append a new term to the cache, enlarging storage if needed. */
static void append_term(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 1024;
        cache = (seq_t *)realloc(cache,
                                 cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Return true iff k² contains every digit of k with at least the same
   multiplicity.  digit_counts is provided by the utils library. */
static bool good_candidate(seq_t k)
{
    seq_t sq = k * k;
    unsigned char cnt_k[10] = {0};
    unsigned char cnt_sq[10] = {0};

    digit_counts(k,   cnt_k);
    digit_counts(sq, cnt_sq);
    for (int d = 0; d < 10; ++d) {
        if (cnt_k[d] > cnt_sq[d]) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* initialise storage on first call */
    if (!initialized) {
        cache_cap   = 1024;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len   = 0;
        last_k      = 0;
        initialized = true;
    }

    /* generate new terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (good_candidate(last_k))
            append_term(last_k);
        ++last_k;
    }

    return cache[n - 1];
}
