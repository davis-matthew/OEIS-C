/*
 * A043367  Duplicate of A023740.
 * https://oeis.org/A043367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 15, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043367: numbers whose base‑5 representation contains
   exactly three digits equal to 4. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms                */
static size_t  cache_cap = 0; /* allocated capacity of the array     */
static idx_t   cached    = 0; /* number of terms currently stored    */
static idx_t   next_th   = 1; /* next integer to test                */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 – n==0 is not used */

    /* Grow cache if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute additional terms until we have n of them */
    while (cached < n) {
        idx_t v = next_th;
        int   c = 0;        /* count of digit 4's in base‑5 */
        idx_t t = v;
        while (t) {
            if ((t % 5) == 4) ++c;
            t /= 5;
        }

        if (c == 3) {
            cache[cached++] = (seq_t)v;
        }
        ++next_th;
    }

    return cache[n - 1];
}
