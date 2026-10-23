/*
 * A156935  G.f. of the z^2 coefficients of the FP2 in the third column of the
 *          A156925 matrix.
 * https://oeis.org/A156935
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A156935:  G.f. of the z^2 coefficients of the FP2 in the A156925 matrix.
   Formula (recurrence, offset 2):
   a(n) = 24*a(n-1) - 254*a(n-2) + 1568*a(n-3) - 6291*a(n-4)
          + 17296*a(n-5) - 33460*a(n-6) + 45984*a(n-7) - 44663*a(n-8)
          + 29976*a(n-9) - 13230*a(n-10) + 3456*a(n-11) - 405*a(n-12). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 2 .. 13, corresponding to indices 0 .. 11. */
    static const seq_t init[12] = {
        -11,          /* a(2) */
        -108,         /* a(3) */
        -425,         /* a(4) */
        720,          /* a(5) */
        25678,        /* a(6) */
        255048,       /* a(7) */
        1901298,      /* a(8) */
        12339600,     /* a(9) */
        73794315,     /* a(10) */
        418519612,    /* a(11) */
        2289264465LL, /* a(12) */
        12207642976LL /* a(13) */
    };

    /* Coefficients of the recurrence (a(n) = Σ coeff[j] * a(n-1-j)). */
    static const seq_t coeff[12] = {
        24,   -254,  1568,  -6291,  17296,
        -33460, 45984, -44663, 29976, -13230,
        3456,  -405
    };

    /* Cache of computed terms, starting from a(2). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* number of terms stored (always ≥ 12). */

    /* Initialise cache with the first 12 terms if not already done. */
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
    }

    /* Convert index to zero‑based position in the cache. */
    size_t pos = (size_t)(n - (idx_t)2);

    if (pos < cache_size)            /* value already known */
        return cache[pos];

    /* Compute terms up to the required position, using the recurrence. */
    while (cache_size <= pos) {
        seq_t sum = 0;
        for (int j = 0; j < 12; ++j) {
            /* sum += coeff[j] * cache[cache_size-1-j]; */
            sum += coeff[j] * cache[cache_size - 1 - j];
        }
        /* Grow the cache array. */
        seq_t *tmp = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
        if (tmp == NULL) {  /* allocation failure – abort */
            /* In a real library we should handle this properly. */
            return 0;
        }
        cache = tmp;
        cache[cache_size] = sum;
        ++cache_size;
    }

    return cache[pos];
}
