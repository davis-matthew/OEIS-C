/*
 * A063323  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          50 ).
 * https://oeis.org/A063323
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13, with initial values
 *            a(2)=–1, a(3)=24, a(4)=46, a(5)=69, a(6)=92, a(7)=115, a(8)=138,
 *            a(9)=162, a(10)=184, a(11)=208, a(12)=230, a(13)=253
 * Range:     i8 n <= 7, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is computed on demand, using a little
    memoisation to achieve an overall O(n) time for the
    largest n requested.  The implementation works for
    both plain integers and arbitrary‑precision
    integral types because it uses only the arithmetic
    operators supplied by seq_t and the realloc
    function for dynamic memory.  */

#include <stdlib.h>

/*  Static cache of already computed values.  */
static seq_t *a_cache = NULL;          /* values of the sequence   */
static unsigned char *a_comp = NULL;   /* 1 if a_cache[i] is valid, 0 otherwise */
static size_t a_cap   = 0;              /* current capacity of a_cache / a_comp */
static size_t a_max   = 0;              /* largest index that is known to be valid */

/*  Expand the cache so that it can hold index n (and all smaller
    indices).  The realloc prototypes are C++‑compatible because we
    explicitly cast the result of realloc.  */
static void
ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= a_cap)
        return;                          /* already large enough */

    size_t new_cap = a_cap ? a_cap * 2 : 64;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    a_comp  = (unsigned char*)realloc(a_comp,  new_cap * sizeof(unsigned char));

    /* initialise the new part of the flag array */
    for (size_t i = a_cap; i < new_cap; ++i)
        a_comp[i] = 0;

    a_cap = new_cap;
}

/*  Initialise the small, predefined values 0..13.  */
static void
fill_base(idx_t i)
{
    /* a_cache[0] and a_cache[1] are regarded as 0; they are not used.  */
    static const seq_t base[14] = {
        (seq_t)0,          /* 0   (unused)  */
        (seq_t)0,          /* 1   (unused)  */
        (seq_t)-1,         /* 2   */
        (seq_t)24,         /* 3   */
        (seq_t)46,         /* 4   */
        (seq_t)69,         /* 5   */
        (seq_t)92,         /* 6   */
        (seq_t)115,        /* 7   */
        (seq_t)138,        /* 8   */
        (seq_t)162,        /* 9   */
        (seq_t)184,        /* 10  */
        (seq_t)208,        /* 11  */
        (seq_t)230,        /* 12  */
        (seq_t)253         /* 13  */
    };
    a_cache[i] = base[i];
    a_comp[i]  = 1;
}

/*  Public interface required by the test harness.  */
seq_t
a(idx_t n)
{
    /* a(n) is defined only for n >= 2.  For indices below the offset
       we return 0, which is harmless for the harness.  */
    if (n < 2)
        return (seq_t)0;

    /* Make sure the internal arrays are large enough.  */
    ensure_capacity(n);

    /* a[0] and a[1] are kept zero but marked as computed so that
       future calls do not attempt to recompute them.  */
    if (!a_comp[0]) { a_cache[0] = 0; a_comp[0] = 1; }
    if (!a_comp[1]) { a_cache[1] = 0; a_comp[1] = 1; }

    /* If the requested index has already been computed, return it.  */
    if (a_comp[n])
        return a_cache[n];

    /* Compute all missing values up to n.  */
    for (size_t i = a_max + 1; i <= (size_t)n; ++i)
    {
        idx_t idx = (idx_t)i;
        if (idx <= 13)            /* initial data (including 0,1 which remain zero) */
            fill_base(idx);
        else                       /* recurrence for n > 13 */
            a_cache[idx] = a_cache[idx - 4] + a_cache[idx - 6] - a_cache[idx - 10];
        a_comp[idx] = 1;
    }

    a_max = (size_t)n;
    return a_cache[n];
}
