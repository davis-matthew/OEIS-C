/*
 * A202830  Expansion of e.g.f. exp(2*x + 3*x^2/2).
 * https://oeis.org/A202830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*(n-1)*a(n-2) with a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, exit */
#include <stdbool.h>  /* bool */

static seq_t *a_cache = NULL;          /* storage for computed terms */
static size_t a_cache_cap = 0;         /* current allocated size */
static bool   a_cache_computed = false;/* flag indicating that a(0)..a_last exist */
static idx_t  a_cache_last   = 0;      /* last index that has been computed */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void a_ensure_cache(idx_t n)
{
    if ((size_t)(n)+1 <= a_cache_cap) return;  /* already enough */
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 1;
    size_t needed  = (size_t)(n) + 1;
    if (needed > new_cap) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE); /* out‑of‑memory */
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/*----------------------------  sequence generator  ------------------------*/
seq_t a(idx_t n)
{
    a_ensure_cache(n);                      /* grow storage if required */

    /* If we need to compute new terms, do so in ascending order. */
    if (!a_cache_computed || n > a_cache_last) {
        idx_t start = a_cache_computed ? a_cache_last + 1 : 0;
        for (idx_t i = start; i <= n; ++i) {
            if (i == 0) {
                a_cache[i] = (seq_t)1;
            } else if (i == 1) {
                a_cache[i] = (seq_t)2;
            } else {
                seq_t coeff = (seq_t)3 * (seq_t)(i - 1);
                a_cache[i] = (seq_t)2 * a_cache[i - 1] + coeff * a_cache[i - 2];
            }
        }
        a_cache_computed = true;
        a_cache_last = n;
    }

    return a_cache[n];
}
