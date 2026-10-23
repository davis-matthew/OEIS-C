/*
 * A137356  a(n) = Sum_{k <= n/2 } binomial(n-2k, 3k).
 * https://oeis.org/A137356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 44, u64 n <= 85, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137356:  a(n) = Sum_{k<=n/2} binomial(n-2k,3k)
   Recurrence: a(0)=…=a(4)=1, a(5)=2 and for n>=6,
                a(n) = 3*a(n-1) – 3*a(n-2) + a(n-3) + a(n-5)       */
/*   This implementation stores computed terms in a dynamic cache
    so that arbitrary order of calls is supported. */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

static seq_t *a_cache = NULL;     /* array of computed values          */
static size_t a_cache_cap = 0;    /* allocated capacity in elements    */
static idx_t  a_max_calculated = (idx_t)-1;  /* highest n computed */

/* Ensure the cache can hold indices 0..max_index inclusive. */
static void ensure_cache_capacity(idx_t max_index)
{
    size_t required = (size_t)max_index + 1;
    if (required <= a_cache_cap)
        return;

    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
    while (new_cap < required)
        new_cap <<= 1;                 /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* failed allocation */

    a_cache = tmp;
    a_cache_cap = new_cap;
}

/* Initialise the first six terms of the sequence.  This is done once
   when the function is first called. */
static void init_base_values(void)
{
    ensure_cache_capacity(5);          /* space for indices 0..5 */
    a_cache[0] = (seq_t)1;
    a_cache[1] = (seq_t)1;
    a_cache[2] = (seq_t)1;
    a_cache[3] = (seq_t)1;
    a_cache[4] = (seq_t)1;
    a_cache[5] = (seq_t)2;
    a_max_calculated = 5;
}

seq_t a(idx_t n)
{
    /* Make room for the first six values if not yet done. */
    ensure_cache_capacity(5);
    if (a_max_calculated == (idx_t)-1)
        init_base_values();

    if (n <= 5)
        return a_cache[(size_t)n];

    /* Compute all missing terms up to n if necessary. */
    if (n > a_max_calculated) {
        ensure_cache_capacity(n);
        for (idx_t i = a_max_calculated + 1; i <= n; ++i) {
            /* a(i) = 3*a(i-1) – 3*a(i-2) + a(i-3) + a(i-5) */
            seq_t term = a_cache[i-1] * (seq_t)3;
            term -= a_cache[i-2] * (seq_t)3;
            term += a_cache[i-3];
            term += a_cache[i-5];
            a_cache[i] = term;
        }
        a_max_calculated = n;
    }

    return a_cache[(size_t)n];
}
