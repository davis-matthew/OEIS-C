/*
 * A211324  Number of (n+1) X (n+1) -3..3 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one or three distinct values.
 * https://oeis.org/A211324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 3*a(n-1) + 2*a(n-2) - 11*a(n-3)
 *                  + a(n-4) + 12*a(n-5) - 2*a(n-6) - 4*a(n-7)
 * with initial values:
 * a(1)=15, a(2)=29, a(3)=55, a(4)=107, a(5)=209,
 * a(6)=409, a(7)=805
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;

    /* Initial terms (indices 1..7) */
    const seq_t init[7] = {
        (seq_t)15, (seq_t)29, (seq_t)55,
        (seq_t)107, (seq_t)209, (seq_t)409, (seq_t)805
    };

    if ((idx_t)0 == n) {        /* undefined; return 0 */
        return (seq_t)0;
    }

    /* Allocate or enlarge cache if necessary */
    if (!cache || cap <= (size_t)n) {
        size_t new_cap = (cap == 0) ? 8 : cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* If already computed up to n, just return */
    if ((idx_t)last >= n) {
        return cache[n];
    }

    /* Compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = init[i - 1];
        } else {
            seq_t v = cache[i - 1] * (seq_t)3;
            v += cache[i - 2] * (seq_t)2;
            v -= cache[i - 3] * (seq_t)11;
            v += cache[i - 4];
            v += cache[i - 5] * (seq_t)12;
            v -= cache[i - 6] * (seq_t)2;
            v -= cache[i - 7] * (seq_t)4;
            cache[i] = v;
        }
    }
    last = n;
    return cache[n];
}
