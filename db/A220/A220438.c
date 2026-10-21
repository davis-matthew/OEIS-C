/*
 * A220438  a(n) = A220437(n)/2.
 * https://oeis.org/A220438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 5 a(n-1) + 35
 *            a(n-2) - 67 a(n-3) - 145 a(n-4) * +145 a(n-5) + 67 a(n-6) - 35
 *            a(n-7) - 5 a(n-8) + a(n-9), * for n ≥ 9, with initial values *
 *            a(0)=3, a(1)=14, a(2)=170, a(3)=1152, a(4)=10326, * a(5)=78963,
 *            a(6)=656624, a(7)=5213426, a(8)=42439104
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[9] = {
    (seq_t)3,
    (seq_t)14,
    (seq_t)170,
    (seq_t)1152,
    (seq_t)10326,
    (seq_t)78963,
    (seq_t)656624,
    (seq_t)5213426,
    (seq_t)42439104
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    if (n < 9)
        return init[n];

    if (cache == NULL) {
        /* Allocate space for the first 9 terms. */
        cap = 9;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
    }

    if (n + 1 > cap) {
        /* Grow the cache to at least n+1 elements (doubling strategy). */
        size_t new_cap = cap;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failure – abort silently */
            return (seq_t)0;
        cache = tmp;

        /* Compute all missing terms up to n. */
        for (size_t i = cap; i <= n; ++i) {
            /* Positive contributions. */
            seq_t add = cache[i-1] * (seq_t)5
                      + cache[i-2] * (seq_t)35
                      + cache[i-5] * (seq_t)145
                      + cache[i-6] * (seq_t)67
                      + cache[i-9];
            /* Negative contributions. */
            seq_t sub = cache[i-3] * (seq_t)67
                      + cache[i-4] * (seq_t)145
                      + cache[i-7] * (seq_t)35
                      + cache[i-8] * (seq_t)5;

            cache[i] = add - sub;               /* final term */
        }
        cap = n + 1;
    }

    return cache[n];
}
