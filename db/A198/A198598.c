/*
 * A198598  Number of 2n X 4 0..3 arrays with values 0..3 introduced in row
 *          major order and each element equal to exactly two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A198598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 4*a(n-2) - 36*a(n-3) + 5*a(n-4) + 27*a(n-5);
 *            initial values: a(1)=1, a(2)=5, a(3)=32, a(4)=251, a(5)=1985
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial values for the first five terms */
static const seq_t init_vals[5] = {
    (seq_t)1,
    (seq_t)5,
    (seq_t)32,
    (seq_t)251,
    (seq_t)1985
};

/* cache for computed terms; cache[0] holds a(1) */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n <= 5) {
        return init_vals[(size_t)n - 1];
    }

    /* ensure cache can hold a(n) */
    size_t needed = (size_t)n;        /* number of terms to store */
    if (cache_len < needed) {
        /* resize cache, cast for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;

        /* fill missing initial terms if required */
        size_t start = cache_len;
        if (start < 5) {
            size_t up = (size_t)5;
            if (up > needed) up = needed;
            for (size_t i = start; i < up; ++i) {
                cache[i] = init_vals[i];
            }
            start = 5;
        }

        /* compute new terms using the recurrence */
        for (size_t i = start; i < needed; ++i) {
            /* compute a(i+1) using cache indices 0‑based */
            seq_t term = (seq_t)9 * cache[i-1];
            term -= (seq_t)4 * cache[i-2];
            term -= (seq_t)36 * cache[i-3];
            term += (seq_t)5 * cache[i-4];
            term += (seq_t)27 * cache[i-5];
            cache[i] = term;
        }
        cache_len = needed;
    }

    return cache[n - 1];
}
