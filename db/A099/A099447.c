/*
 * A099447  An Alexander sequence for the knot 6_3.
 * https://oeis.org/A099447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = A099446(n)-A099446(n-2) Recurrence: for n≥5, a(n) =
 *            3*a(n-1) – 5*a(n-2) + 3*a(n-3) – a(n-4) Base values: a(0) = 1
 *            a(1) = 3 a(2) = 4 a(3) = 0 a(4) = –13 (Derived from
 *            (1‑x)*(1+x)*(1+x²)/(1‑3x+5x²‑3x³+x⁴).)
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure the cache can hold n+1 terms. */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < required) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = 3;
        else if (i == 2)
            cache[2] = 4;
        else if (i == 3)
            cache[3] = 0;
        else {
            seq_t rhs = (i == 4 ? (seq_t)-1 : 0);
            cache[i] = (seq_t)3 * cache[i-1]
                     - (seq_t)5 * cache[i-2]
                     + (seq_t)3 * cache[i-3]
                     - cache[i-4]
                     + rhs;
        }
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
