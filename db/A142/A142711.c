/*
 * A142711  A determinant sequence: M={{a(-1 + n), a(-2 + n), a(-3 + n)},
 *          {a(-2 + n), a(-3 + n), a(-4 + n)}, {a(-3 + n), a(-4 + n), a(-5 +
 *          n)}} a(n)=Det[M].
 * https://oeis.org/A142711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=7. * For n ≥ 6, a(n) =
 *            det([[a(n-1),a(n-2),a(n-3)], * [a(n-2),a(n-3),a(n-4)], *
 *            [a(n-3),a(n-4),a(n-5)]])
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 11, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache[i] = a(i) for i≥0 (cache[0] unused) */
    static size_t cache_size = 0;   /* allocated size (≥ last index + 1) */
    static idx_t last_computed = 0; /* highest index computed so far */

    if (n < 1)
        return (seq_t)0;            /* undefined for n<1 */

    /* Ensure the cache is large enough for index n */
    size_t needed = (size_t)n + 1;
    if (needed < 6)                /* ensure space for the first 5 terms */
        needed = 6;
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)                 /* allocation failed – return 0 */
            return (seq_t)0;
        cache = tmp;
        cache_size = needed;
    }

    /* Initialise the five known base terms once */
    if (last_computed < 5) {
        cache[0] = (seq_t)0;    /* unused slot, kept zero */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)7;
        last_computed = 5;
    }

    /* Return early if within the pre‑computed range */
    if (n <= last_computed)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t s1 = cache[i - 1]; /* a(n-1) */
        seq_t s2 = cache[i - 2]; /* a(n-2) */
        seq_t s3 = cache[i - 3]; /* a(n-3) */
        seq_t s4 = cache[i - 4]; /* a(n-4) */
        seq_t s5 = cache[i - 5]; /* a(n-5) */

        /* Determinant of the 3×3 Toeplitz matrix */
        seq_t det = s1 * (s3 * s5 - s4 * s4)
                  - s2 * (s2 * s5 - s3 * s4)
                  + s3 * (s2 * s4 - s3 * s3);

        cache[i] = det;
    }

    last_computed = n;
    return cache[n];
}
