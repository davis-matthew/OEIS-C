/*
 * A037630  Base-9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0.
 * https://oeis.org/A037630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): 
   a(1)=2, a(2)=21, a(3)=189, a(4)=1703
   a(n)=9*a(n-1)+a(n-3)-9*a(n-4)   (n>=5)   */

/* uses a small cache so repeated calls are fast and order‑independent   */
#include <stdlib.h>
#include <stddef.h>

/* initial values, index 0 unused */
static const seq_t init_vals[5] = {0, 2, 21, 189, 1703};

/* cache boundary (smallest index that has a stored value) */
static idx_t last_computed = 4;               /* a(4) already known */
static seq_t *cache = NULL;                  /* array indexed from 0   */
static idx_t cache_size = 0;                 /* number of elements      */

/* resize the cache to hold indices 0..n  (n is inclusive)  */
static void ensure_cache_capacity(idx_t n)
{
    if (n < cache_size)            /* already large enough */
        return;
    /* allocate n+1 elements (index 0..n) */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        abort(); /* allocation failure – acceptable in contest environment */
    }
    cache = tmp;
    cache_size = (idx_t)new_size;
}

/* main sequence function  */
seq_t a(idx_t n)
{
    if (n <= 4)
        return init_vals[(size_t)n];

    /* ensure cache exists and is large enough to store up to n  */
    if (!cache) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
        cache[4] = init_vals[4];
        cache_size = 5;
    }
    ensure_cache_capacity(n);

    /* compute any terms up to n that are not yet cached  */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = 9*a(i-1) + a(i-3) - 9*a(i-4)  */
        seq_t term = 9 * cache[(size_t)i-1];
        term += cache[(size_t)i-3];
        term -= 9 * cache[(size_t)i-4];
        cache[(size_t)i] = term;
    }
    last_computed = n;
    return cache[(size_t)n];
}
