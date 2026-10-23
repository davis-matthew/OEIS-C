/*
 * A160441  Numerator of Hermite(n, 27/32).
 * https://oeis.org/A160441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27*a(n-1) - 512*(n-1)*a(n-2) with a(0)=1, a(1)=27 This
 *            generates the numerators of Hermite(n, 27/32)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* table of computed a(n) values   */
static size_t cache_cap = 0;     /* current capacity (number of entries)  */

seq_t a(idx_t n) {
    /* Allocate or grow cache as needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure: terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Base cases */
    if (n == 0) {
        cache[0] = 1;
        return 1;
    }
    if (n == 1) {
        cache[0] = 1;          /* ensure a(0) stored for recurrence */
        cache[1] = 27;
        return 27;
    }

    /* Ensure a(0) and a(1) computed */
    cache[0] = 1;
    cache[1] = 27;

    /* Fill cache up to required n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = 27 * cache[i - 1];
        seq_t term2 = 512 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term1 - term2;
    }

    return cache[n];
}
