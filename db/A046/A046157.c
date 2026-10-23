/*
 * A046157  Number of empty intervals when fractional_part(i*gamma) for i = 1,
 *          ..., n is plotted along [ 0, 1 ] subdivided into n equal regions,
 *          where gamma is the Euler-Mascheroni constant.
 * https://oeis.org/A046157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of empty intervals when fractional_part(i*γ) for i
 *            = 1..n is plotted along [0,1] subdivided into n equal regions,
 *            where γ = Euler–Mascheroni constant
 * Range:     u8 n <= 666, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* by definition the OEIS offset is 1; for n < 1 we return 0 */
    if (n == 0)
        return (seq_t)0;

    /* Allocate a marker for each of the n intervals */
    unsigned char *mark = (unsigned char *)calloc((size_t)n, sizeof(unsigned char));
    if (!mark)
        return (seq_t)0;  /* out of memory: graceful fallback */

    /* high‑precision value of the Euler–Mascheroni constant */
    const long double gamma = 0.577215664901532860606512090082402431042L;

    /* Mark the interval visited by each fractional part */
    for (idx_t i = 1; i <= n; ++i)
    {
        long double prod   = (long double)i * gamma;         /* i*γ */
        long double frac   = prod - floorl(prod);             /* fractional part */
        idx_t idx = (idx_t)floorl(frac * (long double)n);    /* interval index */
        mark[(size_t)idx] = 1;
    }

    /* Count the number of empty intervals */
    idx_t empty = 0;
    for (size_t k = 0; k < (size_t)n; ++k)
        if (mark[k] == 0)
            ++empty;

    free(mark);
    return (seq_t)empty;
}
