/*
 * A215635  a(n) = - 12*a(n-1) - 54*a(n-2) - 112*a(n-3) - 105*a(n-4)
 *          -36*a(n-5) - 2*a(n-6), with a(0)=3, a(1)=-6, a(2)=18, a(3)=-60,
 *          a(4)=210, a(5)=-756.
 * https://oeis.org/A215635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -12*a(n-1) - 54*a(n-2) - 112*a(n-3) * -105*a(n-4) -
 *            36*a(n-5) - 2*a(n-6); * Initial terms: a(0)=3, a(1)=-6, a(2)=18,
 *            a(3)=-60, * a(4)=210, a(5)=-756
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 16, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cached values and state */
    static seq_t *cache = NULL;     /* array of computed terms */
    static size_t   stored = 0;     /* number of valid entries (size_valid) */
    static size_t   capacity = 0;   /* allocated array size */

    /* Allocate or grow the cache to hold up to index n */
    if (n >= capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) abort();     /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initial values for the first six terms */
    static const seq_t init[6] = {
        (seq_t)3, (seq_t)-6, (seq_t)18,
        (seq_t)-60, (seq_t)210, (seq_t)-756
    };

    /* Compute terms that have not yet been computed */
    for (size_t i = stored; i <= (size_t)n; ++i) {
        if (i < 6) {
            cache[i] = init[i];
        } else {
            /* Apply the linear recurrence */
            seq_t val = cache[i-1] * (seq_t)-12
                      + cache[i-2] * (seq_t)-54
                      + cache[i-3] * (seq_t)-112
                      + cache[i-4] * (seq_t)-105
                      + cache[i-5] * (seq_t)-36
                      + cache[i-6] * (seq_t)-2;
            cache[i] = val;
        }
    }

    /* Update the number of valid entries */
    stored = (size_t)n + 1;

    return cache[(size_t)n];
}
