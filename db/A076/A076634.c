/*
 * A076634  Coefficient of x^a(n) in (x+1/2)*(x+2/2)*...*(x+n/2) is the
 *          largest one.
 * https://oeis.org/A076634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - r_max where r_max maximizes e_r/2^r, e_r = elementary
 *            symmetric sum of r numbers from 1..n. The algorithm uses
 *            logarithms of e_r to avoid huge integers, and compares log(e_r)
 *            - r*log(2)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   logaddexp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/logaddexp.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate array for logarithms of elementary symmetric sums. */
    long double *log_e = (long double*)malloc((size_t)(n + 1) * sizeof(long double));
    if (!log_e) return 0;
    const long double NEG_INF = -INFINITY;  /* represent -∞ for long double */
    for (idx_t i = 1; i <= n; ++i) log_e[i] = NEG_INF;
    log_e[0] = 0.0L;   /* e_0 = 1 */

    const long double LOG2 = logl(2.0L);

    /* Dynamic programming: build e_r logarithms incrementally. */
    for (idx_t i = 1; i <= n; ++i) {
        long double log_i = logl((long double)i);
        for (idx_t r = i; r >= 1; --r) {
            /* candidate = i * e_{r-1}  →  log(candidate) = log_i + log_e[r-1] */
            long double cand = log_i + log_e[r - 1];
            log_e[r] = logaddexp(log_e[r], cand);
        }
    }

    /* Find r that maximizes log_e[r] - r*log(2). */
    idx_t best_r = 0;
    long double best_val = log_e[0];
    for (idx_t r = 1; r <= n; ++r) {
        long double val = log_e[r] - (long double)r * LOG2;
        if (val > best_val) {
            best_val = val;
            best_r = r;
        }
    }

    seq_t result = (seq_t)(n - best_r);
    free(log_e);
    return result;
}
