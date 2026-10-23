/*
 * A192144  1-sequence of reduction of hexagonal numbers sequence by x^2 ->
 *          x+1.
 * https://oeis.org/A192144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(n) = 4*a(n-1) - 3*a(n-2) -
 *            5*a(n-3) + 5*a(n-4) + 3*a(n-5) - 2*a(n-6) - a(n-7), with initial
 *            values a(1)=0, a(2)=6, a(3)=21, a(4)=77, a(5)=212, a(6)=542,
 *            a(7)=1270
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements that recurrence with memoisation to allow any access order. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of slots currently allocated (index 0 unused) */

    if (n < 1)
        return 0; /* undefined for n<1, but return 0 */

    if (cache_size <= n) {
        /* need more space: grow to n+1 (index 0 unused) */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; terminate */
            abort();
        }
        cache = tmp;
        /* initialise or extend computed values */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 7) {
                static const seq_t init[7] = {0, 6, 21, 77, 212, 542, 1270};
                cache[i] = init[i-1];
            } else {
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                seq_t a4 = cache[i-4];
                seq_t a5 = cache[i-5];
                seq_t a6 = cache[i-6];
                seq_t a7 = cache[i-7];
                seq_t res = a1 * (seq_t)4;
                res -= a2 * (seq_t)3;
                res -= a3 * (seq_t)5;
                res += a4 * (seq_t)5;
                res += a5 * (seq_t)3;
                res -= a6 * (seq_t)2;
                res -= a7;
                cache[i] = res;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
