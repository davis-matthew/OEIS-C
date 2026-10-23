/*
 * A192248  0-sequence of reduction of binomial coefficient sequence
 *          B(n,4)=A000332 by x^2 -> x+1.
 * https://oeis.org/A192248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A192248: 0‑sequence of the reduction of B(n,4)=C(n,4) by the rule  x²→x+1.
    The sequence satisfies the linear recurrence
        a(n) =  6*a(n-1) -10*a(n-2) - 5*a(n-3) +25*a(n-4)
              - 4*a(n-5) -26*a(n-6) + 5*a(n-7) +15*a(n-8)
              - 4*a(n-10) - a(n-11)
    with offset 1 (a(1)=1).  The first 11 terms are stored explicitly.  The
    recurrence holds for n ≥ 12.  The implementation uses memoisation so
    that calls are efficient regardless of order.                       */

#include <stdlib.h>   /* for malloc / realloc */

static const seq_t pre[11] = {
    (seq_t)1, (seq_t)1, (seq_t)16, (seq_t)51, (seq_t)191,
    (seq_t)569, (seq_t)1619, (seq_t)4259, (seq_t)10694,
    (seq_t)25709, (seq_t)59743
};

seq_t a(idx_t n) {
    /* Convert to 0‑based index */
    if (n == (idx_t)0) return (seq_t)0;  /* safety for n<1 */
    idx_t idx = n - 1;

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of elements stored */

    /* Expand cache if necessary */
    if (cache == NULL) {
        /* Allocate space for at least 11 terms */
        cache = (seq_t *)malloc(sizeof(seq_t) * 11);
        for (idx_t i = 0; i < 11; ++i) cache[i] = pre[i];
        cached = 11;
    }
    if (idx < cached) {
        return cache[idx];
    }

    /* Resize cache to accommodate up to index 'idx' */
    if (idx + 1 > cached) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (idx + 1));
        if (tmp == NULL) { /* allocation failure; fall back on precomputed values if available */
            /* In the unlikely event of failure, just return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        cached = idx + 1;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t j = 11; j <= idx; ++j) {
        seq_t pos = 6 * cache[j - 1]
                    + 25 * cache[j - 4]
                    + 5  * cache[j - 7]
                    + 15 * cache[j - 8];
        seq_t neg = 10 * cache[j - 2]
                    + 5  * cache[j - 3]
                    + 4  * cache[j - 5]
                    + 26 * cache[j - 6]
                    + 4  * cache[j -10]
                    + cache[j - 11];
        cache[j] = pos - neg;   /* unsigned subtraction; pos >= neg by the recurrence */
    }

    return cache[idx];
}
