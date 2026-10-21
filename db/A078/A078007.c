/*
 * A078007  Expansion of (1-x)/(1-x-2*x^2-x^3).
 * https://oeis.org/A078007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A078007 – Expansion of (1‑x)/(1‑x‑2x²‑x³)  
   Recurrence: a(0)=1, a(1)=0, a(2)=2, and for n≥3  
                a(n) = a(n‑1) + 2*a(n‑2) + a(n‑3)   */

#include <stdlib.h>

static seq_t *a_cache   = NULL;  /* cache of computed terms   */
static size_t a_len     = 0;     /* number of terms in cache  */
static size_t a_cap     = 0;     /* capacity of cache         */

static void ensure_cache_capacity(idx_t n)
{
    /* Grow the cache until it can hold index n and the first three terms. */
    while (a_cap <= (size_t)n || a_cap < 3) {
        size_t new_cap = a_cap ? a_cap << 1 : 8; /* double, 8 if empty */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory */
        a_cache = tmp;
        a_cap   = new_cap;
    }
    /* Initialise the first three terms if the cache was just created. */
    if (a_len == 0) {
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)0;
        a_cache[2] = (seq_t)2;
        a_len = 3;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)a_len) {           /* already computed        */
        return a_cache[(size_t)n];
    }

    ensure_cache_capacity(n);

    for (size_t i = a_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            a_cache[i] = (seq_t)1;
        } else if (i == 1) {
            a_cache[i] = (seq_t)0;
        } else if (i == 2) {
            a_cache[i] = (seq_t)2;
        } else {
            a_cache[i] = a_cache[i-1] + (seq_t)2 * a_cache[i-2] + a_cache[i-3];
        }
    }
    a_len = (size_t)n + 1;   /* cache now covers 0..n        */
    return a_cache[(size_t)n];
}
