/*
 * A194752  Number of k such that {-k*e} < {-n*e}, where { } = fractional
 *          part.
 * https://oeis.org/A194752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + #{ 1 <= k < n : {k*e} > {n*e} }, where {x} = x -
 *            floor(x) and e is Euler's number. k = 0 is always counted (since
 *            {0*e} = 0 and {0 - n*e} < { -n*e } for all n)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1, but return 0 */

    /* allocate array for fractional parts 0..n */
    /* long double has sufficient precision for n <= 10000 */
    long double *r = (long double *)malloc(sizeof(long double) * (n + 1));
    if (!r)
        return 0;               /* allocation failed */

    const long double E = 2.7182818284590452353602874713526624977572L;

    r[0] = 0.0L;
    long double cur = 0.0L;
    for (idx_t i = 1; i <= n; ++i) {
        cur += E;
        if (cur >= 1.0L)
            cur -= floorl(cur);   /* reduce modulo 1, keeping high precision */
        r[i] = cur;
    }

    long double rn = r[n];
    seq_t count = 1;                /* k = 0 always counted */
    for (idx_t k = 1; k < n; ++k) {
        if (r[k] > rn)
            ++count;
    }

    free(r);
    return count;
}
