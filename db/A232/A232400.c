/*
 * A232400  Number of (n+1)X(2+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          vertically, with no adjacent elements equal.
 * https://oeis.org/A232400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) + 4*a(n-3) - 3*a(n-4) - 12*a(n-5) - 3*a(n-6) +
 *            6*a(n-7) + 6*a(n-8) + 2*a(n-9) (n >= 10) with initial terms:
 *            a(1)=4, a(2)=16, a(3)=20, a(4)=72, a(5)=124, a(6)=356, a(7)=664,
 *            a(8)=1808, a(9)=3572. The function caches computed values so it
 *            works for any call order
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated size */
    static size_t computed = 0;        /* largest n computed (0 means none) */

    /* Convert idx_t (unsigned) to size_t for indexing; n is at least 1 by definition */
    size_t N = (size_t)n;

    /* Ensure cache includes first 9 terms */
    if (cache_size < 9) {
        size_t new_size = (N > 9) ? N : 9;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
        /* Initialize first 9 terms */
        static const seq_t init[9] = { (seq_t)4, (seq_t)16, (seq_t)20, (seq_t)72,
                                       (seq_t)124, (seq_t)356, (seq_t)664,
                                       (seq_t)1808, (seq_t)3572 };
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        computed = 9;
    }

    /* Expand cache if needed */
    if (N > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, N * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure, return 0 */
        cache = tmp;
        cache_size = N;
    }

    /* If we have already computed up to N, just return */
    if (computed >= N)
        return cache[N - 1];

    /* Coefficients and corresponding delays (n - delay) from recurrence */
    static const int coeff[8]  = {  5,   4, -3, -12,  -3,   6,   6,   2 };
    static const idx_t delay[8] = {  2,   3,   4,    5,   6,   7,   8,   9 };

    /* Compute missing terms up to N */
    for (size_t i = computed + 1; i <= N; ++i) {
        seq_t pos = 0;   /* sum of positive contributions */
        seq_t neg = 0;   /* sum of negative contributions */

        for (int j = 0; j < 8; ++j) {
            idx_t d = delay[j];
            if (i > d) { /* i - d >= 1 */
                seq_t val = cache[i - 1 - d];
                int c = coeff[j];
                if (c > 0)
                    pos += val * (seq_t)c;
                else
                    neg += val * (seq_t)(-c);
            }
        }
        cache[i - 1] = pos - neg;  /* pos >= neg by definition of sequence */
    }

    computed = N;
    return cache[N - 1];
}
