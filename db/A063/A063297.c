/*
 * A063297  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          24 ).
 * https://oeis.org/A063297
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-2) + a(n-4) – a(n-6) for n > 8 * Initial values
 *            (offset 2): * a(2) = –1, a(3) = 5, a(4) = 12, a(5) = 17, * a(6)
 *            = 26, a(7) = 31, a(8) = 40
 * Range:     i8 n <= 21, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* ----- static cache ----------------------------------------------------- */
static seq_t *cache      = NULL;  /* allocated array of computed terms   */
static size_t cache_cap  = 0;     /* current capacity of cache (index+1) */
static idx_t  cache_last = 1;     /* highest index that has been computed */

/* ----- minimal initial values ------------------------------------------ */
static const seq_t init_vals[9] = {
    0,          /* unused 0 */
    0,          /* unused 1 */
    -1,         /* a(2) */
    5,          /* a(3) */
    12,         /* a(4) */
    17,         /* a(5) */
    26,         /* a(6) */
    31,         /* a(7) */
    40          /* a(8) */
};

/* ----- sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* offset: only n >= 2 is defined */
    if (n < 2) return 0;

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failed – give up */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute yet-uncomputed values */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 8) {
            /* use the hard‑coded initial terms */
            cache[i] = init_vals[(size_t)i];
        } else {
            /* recurrence: a(i) = a(i-2) + a(i-4) – a(i-6) */
            cache[i] = cache[i-2] + cache[i-4] - cache[i-6];
        }
    }
    cache_last = n;

    return cache[n];
}
