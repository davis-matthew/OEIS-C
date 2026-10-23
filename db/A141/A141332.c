/*
 * A141332  a(n) = number of positive integers k, k < n, where |the sine of k
 *          radians| is < |the sine of n radians|. Also, a(n) = number of
 *          positive integers k, k < n, where |the cosine of k radians| is >
 *          |the cosine of n radians|.
 * https://oeis.org/A141332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k | 1 <= k < n AND |sin(k radians)| < |sin(n radians)|
 *            }|
 * Range:     u8 n <= 277, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

static double *sines = NULL;   /* sines[i] = sin(i) (radians), i>=0 */
static size_t sin_alloc = 0;   /* allocated size */

/* Compute a(n) as defined in A141332 */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;                /* offset starts at n = 1 */

    /* Grow the sine table if necessary */
    if ((size_t)n >= sin_alloc) {
        size_t newsize = (size_t)n + 1;   /* need indices up to n */
        double *tmp = (double *)realloc(sines, newsize * sizeof(double));
        if (!tmp) return 0;              /* allocation failed, give up */
        sines = tmp;
        for (size_t i = sin_alloc; i < newsize; ++i)
            sines[i] = sin((double)i);
        sin_alloc = newsize;
    }

    double target = fabs(sines[n]);      /* |sin(n)| */
    seq_t count = 0;
    for (size_t i = 1; i < (size_t)n; ++i)
        if (fabs(sines[i]) < target)
            ++count;

    return count;
}
