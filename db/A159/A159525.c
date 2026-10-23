/*
 * A159525  Numerator of Hermite(n, 9/16).
 * https://oeis.org/A159525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 9, * a(n) = 9*a(n-1) - 128*(n-1)*a(n-2) for n
 *            >= 2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache handling ----------------------------------------------------------- */
static seq_t *cache   = NULL; /* stored values */
static idx_t  cap     = 0;    /* capacity of the cache */
static idx_t  len     = 0;    /* number of values actually stored */

/* Ensure the cache can hold at least (n+1) elements */
static void ensure_capacity(idx_t n)
{
    if (cap > n)
        return;                 /* already large enough */

    idx_t newcap = (cap) ? cap : 1;
    while (newcap <= n)
        newcap <<= 1;              /* grow until enough */

    /* realloc may return void* in C++; cast to seq_t* explicitly */
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp && newcap > 0)        /* allocation failed */
        exit(EXIT_FAILURE);

    cache = tmp;
    cap   = newcap;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialize on first call */
    if (cache == NULL) {
        ensure_capacity(1);          /* reserve space for indices 0 and 1 */
        cache[0] = 1;                /* a(0) */
        cache[1] = 9;                /* a(1) */
        len     = 2;                 /* two values stored */
    }

    if (n < len)                     /* value already computed */
        return cache[n];

    /* compute up to n */
    ensure_capacity(n);
    for (idx_t k = len; k <= n; ++k) {
        seq_t factor = (seq_t)128 * ((seq_t)(k - 1));   /* 128*(k-1)   */
        seq_t term1  = cache[k - 1] * ((seq_t)9);       /* 9*a(k-1)    */
        seq_t term2  = cache[k - 2] * factor;           /* 128*(k-1)*a(k-2) */
        cache[k] = term1 - term2;                       /* recurrence   */
    }
    len = n + 1;
    return cache[n];
}
