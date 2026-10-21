/*
 * A226194  Expansion of f(-x^1, -x^7) * f(-x^3, -x^5) in powers of x where
 *          f(, ) is Ramanujan's general theta function.
 * https://oeis.org/A226194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Expansion of f(-x^1,-x^7)*f(-x^3,-x^5) in powers of x G.f. =
 *            Product_{k>0} (1 - x^(8k))^2 / (1 + x^k)
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached coefficients */
static size_t cache_size = 0;        /* size of cache */

/* Compute coefficients up to n (inclusive) */
static void compute_series(idx_t n)
{
    size_t N = (size_t)n;
    size_t size = N + 1;

    seq_t *a = (seq_t*)malloc(size * sizeof(seq_t));
    seq_t *b = (seq_t*)malloc(size * sizeof(seq_t));
    if (!a || !b) return;          /* allocation failed */

    /* initialise: coefficient of x^0 is 1, the rest 0 */
    a[0] = 1;
    for (size_t i = 1; i <= N; ++i) a[i] = 0;

    /* iterate over factors (1 - x^m)^c_m where
       c_m = 1 when m is odd, c_m = 2 when m is divisible by 8, c_m = 0 otherwise */
    for (size_t m = 1; m <= N; ++m) {
        int c = 0;
        if (m & 1)            c = 1;          /* odd m */
        else if (m % 8 == 0)  c = 2;          /* divisible by 8 */
        else                  continue;       /* skip */

        for (size_t i = 0; i <= N; ++i) {
            seq_t val = a[i];
            if (i >= m) {
                if (c == 1)
                    val -= a[i - m];
                else  /* c == 2 */
                    val -= (seq_t)2 * a[i - m];
            }
            if (c == 2 && i >= 2 * m)
                val += a[i - 2 * m];
            b[i] = val;
        }
        /* swap a and b */
        seq_t *tmp = a;
        a = b;
        b = tmp;
    }

    /* a now holds the desired coefficients */
    if (cache) free(cache);
    cache = a;
    cache_size = size;
    free(b);           /* the unneeded buffer */
}

seq_t a(idx_t n)
{
    if (n >= (idx_t)cache_size || cache == NULL)
        compute_series(n);
    return cache[(size_t)n];
}
