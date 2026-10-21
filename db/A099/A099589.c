/*
 * A099589  Expansion of x^3 / (1 - 4*x + 6*x^2 - 4*x^3 + 2*x^4).
 * https://oeis.org/A099589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 2*a(n-4) for n > 4;
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=4
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 36, i64 n <= 74, i128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache = NULL;
    static idx_t max_computed = 4;
    static size_t cap = 0;

    if (!initialized) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)4;
        initialized = true;
    }

    if (n <= max_computed) {
        return cache[n];
    }

    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; return zero as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)4 * cache[i-1]
            - (seq_t)6 * cache[i-2]
            + (seq_t)4 * cache[i-3]
            - (seq_t)2 * cache[i-4];
    }
    max_computed = n;
    return cache[n];
}
