/*
 * A061202  (tau<=)_4(n).
 * https://oeis.org/A061202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} tau_3(k) * floor(n/k), * where tau_3(k) =
 *            Sum_{d|k} tau(d), * tau(d) = number of divisors of d. * The
 *            function implements this by computing tau and tau_3 with * a
 *            sieve in O(n log n) time and then accumulating the sum
 * Range:     u8 n <= 19, u16 n <= 771, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* idx_t and seq_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1, but guard against 0 */

    /* Allocate arrays for divisor counts.  idx_t is a POD type, so
       malloc works safely.  tau(k) and tau_3(k) stay very small
       (for n <= 10000 they fit comfortably in 64 bits).   */
    idx_t *d   = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    idx_t *t3  = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!d || !t3) {
        free(d); free(t3);
        return 0;                    /* memory exhaustion case */
    }

    /* Initialise to 0. */
    for (idx_t i = 1; i <= n; ++i) {
        d[i]  = 0;
        t3[i] = 0;
    }

    /* Compute tau(k): number of divisors of k. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            ++d[j];
        }
    }

    /* Compute tau_3(k) = Sum_{d|k} tau(d). */
    for (idx_t a = 1; a <= n; ++a) {
        idx_t add = d[a];
        for (idx_t j = a; j <= n; j += a) {
            t3[j] += add;
        }
    }

    /* Accumulate the final result. */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        result += (seq_t)t3[k] * (seq_t)(n / k);
    }

    free(d);
    free(t3);
    return result;
}
