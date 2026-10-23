/*
 * A173645  Partial sums of floor(n^2/11).
 * https://oeis.org/A173645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^n floor(k^2 / 11)
 * Range:     u8 n <= 15, u16 n <= 128, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(n)=a(n-1)+floor(n^2/11). */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n) {
    /* Static cache to handle arbitrary call order efficiently. */
    static seq_t *cache = NULL;          /* array of a(0)...a(max_computed) */
    static idx_t  max_computed = (idx_t)-1;  /* index of the last computed entry */

    /* Allocate cache if this is the first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!cache) return (seq_t)0;     /* allocation failed, return 0 as fallback */
        cache[0] = (seq_t)0;             /* a(0) = 0 */
        max_computed = 0;
    }

    /* Extend cache up to n if necessary. */
    if (n > max_computed) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* realloc failed */
        cache = tmp;

        /* Compute new values iteratively. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t sq = (seq_t)i * (seq_t)i;        /* i^2 in seq_t arithmetic */
            seq_t term = sq / (seq_t)11;           /* floor(i^2 / 11) */
            cache[i] = cache[i - 1] + term;
        }
        max_computed = n;
    }

    return cache[n];
}
