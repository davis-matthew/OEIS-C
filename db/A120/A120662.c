/*
 * A120662  Expansion of x*(12 +101*x -189*x^2)/((1+2*x)*(1-3*x)*(1-5*x)).
 * https://oeis.org/A120662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=12, a(2)=173, a(3)=861; For n≥4:
 *            a(n)=6*a(n-1)+a(n-2)-30*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;       /* cache of computed terms */
static idx_t  a_cache_cap = 0;      /* allocated capacity (in elements) */
static idx_t  a_cache_sz  = 0;      /* number of terms stored (next free index) */

static void grow_cache(idx_t n)
{
    /* Ensure capacity for indices 0..n inclusive */
    if (n < a_cache_sz)
        return;
    if (a_cache_cap == 0)
        a_cache_cap = 1;
    while (a_cache_cap <= n)
        a_cache_cap <<= 1;          /* double until enough */

    seq_t *new_ptr = (seq_t *)realloc(a_cache, a_cache_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();                    /* memory exhausted */
    a_cache = new_ptr;

    /* Compute missing values */
    for (idx_t i = a_cache_sz; i <= n; ++i) {
        if (i == 0)
            a_cache[i] = (seq_t)0;
        else if (i == 1)
            a_cache[i] = (seq_t)12;
        else if (i == 2)
            a_cache[i] = (seq_t)173;
        else if (i == 3)
            a_cache[i] = (seq_t)861;
        else
            a_cache[i] = (seq_t)6 * a_cache[i-1] + a_cache[i-2]
                       - (seq_t)30 * a_cache[i-3];
    }
    a_cache_sz = n + 1;
}

seq_t a(idx_t n)
{
    grow_cache(n);
    return a_cache[n];
}
