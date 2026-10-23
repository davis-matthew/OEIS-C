/*
 * A025973  Expansion of 1/((1-2*x)*(1-4*x)*(1-7*x)*(1-10*x)).
 * https://oeis.org/A025973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(n+1)-2^(n+1))/2 + 17*a(n-1) - 70*a(n-2), a(0) = 1,
 *            a(1) = 23 (This is a linear recurrence derived from the
 *            generating function.)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *cache = NULL;      /* cached values of a(n)             */
static size_t cache_cap = 0;     /* allocated capacity in cache        */
static size_t cache_len = 0;     /* number of values actually stored   */

/* next values of 2^(k) and 4^(k) for k = current_index+1 */
static seq_t pow2_next = (seq_t)2;   /* 2^1 initially                     */
static seq_t pow4_next = (seq_t)4;   /* 4^1 initially                     */
static int pow_initialized = 0;     /* have we set pow2_next/4_next?      */

/* Ensure the cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_len)          /* already have enough */
        return;

    /* Resize the buffer if needed */
    size_t new_cap = cache_cap ? cache_cap : 2;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;

    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) exit(1);              /* out of memory */

    /* Initialization on first run */
    if (!pow_initialized) {
        pow2_next = (seq_t)2;           /* 2^1 */
        pow4_next = (seq_t)4;           /* 4^1 */
        pow_initialized = 1;
    }

    /* Compute new terms incrementally */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)23;
        } else {
            /* term = (4^(i+1) - 2^(i+1))/2 + 17*a(i-1) - 70*a(i-2) */
            seq_t term = (pow4_next - pow2_next) / (seq_t)2;
            term = term + (seq_t)17 * new_buf[i - 1];
            term = term - (seq_t)70 * new_buf[i - 2];
            val = term;
        }
        new_buf[i] = val;

        /* Update powers for the next index */
        pow2_next = pow2_next * (seq_t)2;  /* 2^(i+2) */
        pow4_next = pow4_next * (seq_t)4;  /* 4^(i+2) */
    }

    cache     = new_buf;
    cache_cap = new_cap;
    cache_len = (size_t)n + 1;
}

/* Return the OEIS sequence A025973 term a(n). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
