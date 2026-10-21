/*
 * A054444  Even-indexed terms of A001629(n), n >= 2, (Fibonacci convolution).
 * https://oeis.org/A054444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ( (2*n+1)*F(2*(n+1)) + 4*(n+1)*F(2*n+1) ) / 5 (a
 *            Fibonacci-convolution recurrence: a(0)=1, a(1)=5, a(2)=20,
 *            a(3)=71, a(n)=6*a(n-1)-11*a(n-2)+6*a(n-3)-a(n-4) for n≥4)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* initial seed for n = 0..3 */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)5;          /* a(1) */
        cache[2] = (seq_t)20;         /* a(2) */
        cache[3] = (seq_t)71;         /* a(3) */
        cache_len = 4;
    }

    /* grow cache if required */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        /* double until enough */
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    while (cache_len <= (size_t)n) {
        /* recurrence: a_k = 6*a_{k-1} - 11*a_{k-2} + 6*a_{k-3} - a_{k-4} */
        seq_t a1 = cache[cache_len - 1];
        seq_t a2 = cache[cache_len - 2];
        seq_t a3 = cache[cache_len - 3];
        seq_t a4 = cache[cache_len - 4];
        seq_t next = (seq_t)6 * a1;
        next -= (seq_t)11 * a2;
        next += (seq_t)6 * a3;
        next -= a4;
        cache[cache_len++] = next;
    }

    return cache[n];
}
