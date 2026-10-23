/*
 * A182191  a(n) = 6*a(n-1) - a(n-2) + 12 with n>1, a(0)=-1, a(1)=5.
 * https://oeis.org/A182191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) + 12 Initial values: a(0) = -1, a(1) =
 *            5 Recurrence valid for n > 1
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 12, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_computed = (idx_t)-1;

    /* First call: allocate space and initialize base cases */
    if (max_computed == (idx_t)-1) {
        capacity = (n < 1 ? 2 : n + 1);
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)5;
        max_computed = 1;
    }
    /* Ensure the array is large enough for index n */
    else if (n > capacity - 1) {
        size_t new_cap = capacity;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute any terms that have not yet been calculated */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * 6 - cache[i-2] + 12;
    }
    if (n > max_computed) max_computed = n;

    return cache[n];
}
