/*
 * A099839  a(n) = -10*a(n-1) - 5*a(n-2) with a(0) = 1, a(1) = -11.
 * https://oeis.org/A099839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -10*a(n-1) - 5*a(n-2) with a(0)=1, a(1)=-11
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes values by iterative recurrence, memoising results with a static array. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised results */
    static seq_t *cache   = NULL;
    static size_t  sz     = 0;      /* current number of stored terms */
    static size_t  cap    = 0;      /* allocated capacity */

    /* minimal required size to hold term n */
    size_t need = (size_t)n + 1;

    /* allocate or grow cache if necessary */
    if (cap < need) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* In both C and C++ realloc may return NULL on failure.
           For the purposes of the sequence generator we assume
           sufficient memory; otherwise the program would terminate. */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise first two terms if cache is empty */
    if (sz == 0) {
        cache[0] = (seq_t)1;
        cache[1] = -(seq_t)11;
        sz = 2;
    }

    /* compute beyond the current cache */
    if (need > sz) {
        for (size_t i = sz; i < need; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            /* a(i) = -10*a(i-1) - 5*a(i-2) */
            cache[i] = -( (seq_t)10 * prev1 ) - ( (seq_t)5 * prev2 );
        }
        sz = need;
    }

    return cache[(size_t)n];
}
