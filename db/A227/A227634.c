/*
 * A227634  Least splitter of log(n) and log(n+1).
 * https://oeis.org/A227634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = the least positive integer d such that there exists an
 *            integer c with log(n) ≤ c/d < log(n+1)
 * Range:     u8 n <= 401, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation simply scans d = 1,2,3,…,
    checking the inequality with double arithmetic. */
/*  This works because, for n up to about 10 000, the required
    denominator is at most n, and double precision is ample
    to distinguish the strict inequality. */
#include <math.h>
#include <stdint.h>

/* External linkage – implements the required function */
seq_t a(idx_t n)
{
    if (n < 1)         /* undefined for n == 0 */
        return (seq_t)0;

    /* Pre‑compute the two logarithms.  The values are positive
       and only need double precision for the comparison below. */
    double ln_n  = log((double)n);
    double ln_n1 = log((double)(n + 1));

    /* Epsilon to guard against rounding errors when
       multiplying by an integer. */
    const double eps = 1e-12;

    for (uint64_t d = 1;; ++d) {
        /* smallest integer ≥ d*ln_n  */
        double min_c = ceil(d * ln_n - eps);
        /* largest integer < d*ln_n1   */
        double max_c = floor(d * ln_n1 + eps);

        if (min_c <= max_c)          /* an integer c exists */
            return (seq_t)d;
    }
}
