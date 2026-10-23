/*
 * A175019  a(0)=a(1)=1; a(n) = a(n-a(n-2)) + 2.
 * https://oeis.org/A175019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A175019
   a(0) = a(1) = 1
   a(n) = a(n - a(n-2)) + 2
   (n >= 2)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- Static cache for memoisation -------------------------------- */
static seq_t *cache   = NULL;   /* array holding computed values   */
static size_t  cap    = 0;      /* current allocated capacity      */
static idx_t   last   = 0;      /* highest index computed so far   */

/* ensure that the cache can store indices 0 .. needed (inclusive)   */
static void grow_cache(idx_t needed)
{
    if (needed + 1 <= cap)
        return;   /* already enough space */

    size_t new_cap = (cap ? cap * 2 : 8);   /* start with a small heap  */
    while (new_cap <= (size_t)(needed + 1))
        new_cap <<= 1;                       /* grow until fit  */

    /* realloc the array (C and C++ compatible cast) */
    seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)   /* allocation failed */
        exit(EXIT_FAILURE);

    cache = tmp;
    cap   = new_cap;
}

/* Make sure the cache holds values 0 … n.  Compute any new ones.      */
static void ensure_cache(idx_t n)
{
    grow_cache(n);

    /* initialise base cases if we have never computed anything yet   */
    if (last == 0) {
        cache[0] = (seq_t)1;
        if (cap > 1)
            cache[1] = (seq_t)1;
        last = 1;
    }

    if (last >= n)
        return;   /* already ready */

    /* sequentially compute all missing values                         */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* i >= 2 because i > last >= 1 */
        seq_t prev2 = cache[i - 2];          /* a(i-2)          */
        idx_t  idx  = i - (idx_t)prev2;      /* i - a(i-2)      */
        seq_t val   = cache[idx] + (seq_t)2; /* a(idx) + 2      */
        cache[i]    = val;
    }
    last = n;
}

/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    ensure_cache(n);
    return cache[n];
}
