/*
 * A226322  Number of tilings of a 4 X n rectangle using L tetrominoes and 2 X
 *          2 tiles.
 * https://oeis.org/A226322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   p_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -p_n + 5*a(n-2)+6*a(n-3)+4*a(n-4)-a(n-6)-3*a(n-8)-2*a(n-9)-4*a(n-10)-2*a(n-12)
   with p_0 = -1, p_2 = +2, p_6 = +1, and p_n = 0 otherwise. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/p_value.h"

static seq_t *cache = NULL;
static size_t cache_size       = 0; /* number of terms currently stored */
static size_t cache_capacity   = 0; /* allocated size */

/* Coefficients of the denominator polynomial Q(x) = Σ c_k x^k. */
static const int c[13] = {
    -1,    /* c0  */
     0,    /* c1  */
     5,    /* c2  */
     6,    /* c3  */
     4,    /* c4  */
     0,    /* c5  */
    -1,    /* c6  */
     0,    /* c7  */
    -3,    /* c8  */
    -2,    /* c9  */
    -4,    /* c10 */
     0,    /* c11 */
    -2     /* c12 */
};

/* Ensure the internal cache can store values up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_capacity < needed) {
        size_t newcap = cache_capacity ? cache_capacity * 2 : 16;
        if (newcap < needed) newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();           /* memory allocation failed */
        cache      = tmp;
        cache_capacity = newcap;
    }
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n < cache_size) return cache[n];

    /* Grow internal cache if required. */
    ensure_capacity(n);

    /* Compute terms from cache_size up to n. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t term = 0;                  /* Σ_{k=1}^{12} c_k * a_{i-k} */

        for (int k = 1; k <= 12; ++k) {
            if (i >= (size_t)k) {
                if (c[k] > 0)
                    term += (seq_t)c[k] * cache[i - k];
                else if (c[k] < 0)
                    term -= (seq_t)(-c[k]) * cache[i - k];
            }
        }

        int pn = p_value(i);
        if (pn != 0) {
            if (pn > 0)
                term -= (seq_t)pn;      /* subtract positive pn */
            else
                term += (seq_t)(-pn);   /* add |pn| when pn<0 */
        }

        cache[i] = term;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
