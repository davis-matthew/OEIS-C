/*
 * A227779  Least splitter of s(n) and s(n+1), where s(n) = sum{(k +
 *          1/2)^(-1/2), k >= 1}.
 * https://oeis.org/A227779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the least positive integer d such that s(n) <= c/d <
 *            s(n+1) for some integer c, where s(n) = sum_{k=1}^{n} (k +
 *            ½)^(-½)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Internal static cache for the partial sums of s(n). */
static idx_t cache_alloc = 0;          /* allocated slots (sums[0 .. cache_alloc-1]) */
static long double *sums = NULL;

/* Expand the sums array to cover indices up to 'max_idx' (inclusive). */
static void ensure_sums(idx_t max_idx)
{
    if (cache_alloc > max_idx) return;             /* already covered */

    /* Allocate enough space. */
    idx_t new_alloc = max_idx + 1;
    long double *tmp = (long double *)realloc(sums, new_alloc * sizeof(long double));
    if (!tmp) abort();                              /* out of memory */
    sums = tmp;

    /* Compute missing partial sums.  */
    for (idx_t i = cache_alloc; i <= max_idx; ++i) {
        if (i == 0) {
            sums[i] = 0.0L;                        /* s(0) = 0 */
        } else {
            sums[i] = sums[i-1] + 1.0L / sqrtl((long double)i + 0.5L);
        }
    }
    cache_alloc = new_alloc;
}

/* Public sequence function.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                           /* undefined for n=0 */

    /* We need s(n) and s(n+1). */
    ensure_sums(n + 1);

    long double s_n  = sums[n];
    long double s_n1 = sums[n+1];

    /* Check d = 1 first. */
    if ((long long)floorl(s_n1) > (long long)floorl(s_n)) {
        return (seq_t)1;
    }

    /* Search for the smallest d > 1.  The required d is always < 1000
       for n up to 10000, so this linear search is fast. */
    const long double eps = 1e-18L;                /* safety margin against rounding */
    for (idx_t d = 2; ; ++d) {
        long double low  = s_n  * d;
        long double high = s_n1 * d;
        idx_t ceil_low  = (idx_t)ceill(low  - eps);   /* smallest integer ≥ low   */
        idx_t floor_high = (idx_t)floorl(high - eps); /* largest integer < high  */
        if (ceil_low <= floor_high) {
            return (seq_t)d;
        }
        /* Safety break – in practice this never triggers for the
           tested range, but it protects against infinite loops. */
        if (d > 100000) return (seq_t)d;
    }
}
