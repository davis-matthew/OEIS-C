/*
 * A097787  a(n)=3a(n-1)+C(n+4,4),n>0, a(0)=1.
 * https://oeis.org/A097787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(n) = 3*a(n-1) + C(n+4,4) , n>0 * (C(n+4,4) =
 *            (n+4)(n+3)(n+2)(n+1)/24)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term of OEIS A097787.  The function may be called in
 * any order; it keeps a small cache to avoid recomputing previous
 * values.  The implementation uses only integer arithmetic. */
seq_t a(idx_t n)
{
    /* static cache to memoize values between calls */
    static seq_t *cache = NULL;  /* array of terms a(0) .. a(m) */
    static size_t cached_size = 0;   /* number of allocated slots */
    static idx_t last_computed = 0;  /* highest n whose value is valid */

    /* ensure cache has room for a(n) */
    if (cache == NULL) {
        cached_size = (size_t)n + 1;
        cache = (seq_t *)malloc(cached_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* fallback on allocation failure */
        cache[0] = (seq_t)1;                 /* a(0) */
        last_computed = 0;
    } else if (cached_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        /* initialise newly allocated slots to 0 */
        for (size_t i = cached_size; i < new_size; ++i) cache[i] = 0;
        cached_size = new_size;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* binomial C(i+4,4) = (i+4)(i+3)(i+2)(i+1)/24 */
        seq_t binom = ((seq_t)(i + 4)) *
                      ((seq_t)(i + 3)) *
                      ((seq_t)(i + 2)) *
                      ((seq_t)(i + 1)) / (seq_t)24;
        cache[i] = cache[i - 1] * (seq_t)3 + binom;
    }
    last_computed = n;
    return cache[n];
}
