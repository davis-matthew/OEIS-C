/*
 * A075536  a(n) = ((1+(-1)^n)*T(n+1) + (1-(-1)^n)*S(n))/2, where T(n) =
 *          tribonacci numbers A000073, S(n) = generalized tribonacci numbers
 *          A001644.
 * https://oeis.org/A075536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A075536
   Recurrence: a(0)=0, a(1)=1, a(2)=1, a(3)=7, a(4)=4, a(5)=21
                a(n) = 3*a(n-2) + a(n-4) + a(n-6)   for n≥6
   The function caches computed values for repeated calls.
*/
#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* allocated array length */
    static idx_t max_computed = (idx_t)-1; /* highest index computed, (idx_t)-1 = 0xFFFFFFFFFFFFFFFF */
    static int initialized = 0;

    /* one-time initialization */
    if (!initialized) {
        cache_len = 16;                      /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0;                /* memory allocation failed; fall back to 0 */
        /* predefined first six terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)21;
        max_computed = 5;
        initialized = 1;
    }

    /* expand cache if needed */
    if (n >= cache_len) {
        idx_t new_len = cache_len ? cache_len << 1 : 16;
        while (new_len <= n) new_len <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!cache) return 0;                /* memory allocation failed; fall back to 0 */
        cache_len = new_len;
    }

    /* compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i < 6) {
            /* first six terms already set; no action needed */
        } else {
            /* recurrence: a(i) = 3*a(i-2) + a(i-4) + a(i-6) */
            seq_t v1 = cache[i-2];
            seq_t v2 = cache[i-4];
            seq_t v3 = cache[i-6];
            cache[i] = (seq_t)3 * v1 + v2 + v3;
        }
        max_computed = i;                 /* update highest computed */
    }

    return cache[n];
}
