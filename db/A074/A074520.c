/*
 * A074520  a(n) = 1^n + 6^n + 7^n.
 * https://oeis.org/A074520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 6^n + 7^n (OEIS A074520)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed values */
static idx_t  cache_cap = 0;         /* current capacity (number of elements) */
static idx_t  max_comp  = 0;         /* highest n for which cache[n] is valid */
static int    initialized = 0;       /* whether base terms have been stored */

/* Ensure that the cache can hold indices 0..need */
static void ensure_capacity(idx_t need)
{
    if (need + 1 <= cache_cap) return;
    size_t new_cap = (size_t)need + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out‑of‑memory: terminate quietly */
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* Store the two initial terms of the sequence */
static void init_base(void)
{
    if (initialized) return;
    if (cache_cap < 2) ensure_capacity(1);
    cache[0] = (seq_t)3;   /* a(0) */
    cache[1] = (seq_t)14;  /* a(1) */
    max_comp = 1;
    initialized = 1;
}

/* Compute a(n) = 1^n + 6^n + 7^n using the recurrence
   a(n) = 13*a(n-1) - 42*a(n-2) + 30   for n > 1   */
seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)14;

    ensure_capacity(n);
    init_base();

    /* Extend the cache up to n if necessary */
    if (max_comp < n) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_t val = cache[i-1] * (seq_t)13;
            val += (seq_t)30;
            val -= cache[i-2] * (seq_t)42;
            cache[i] = val;
        }
        max_comp = n;
    }
    return cache[n];
}
