/*
 * A033810  Number of people needed so that probability of at least two
 *          sharing a birthday out of n possible days is at least 50%.
 * https://oeis.org/A033810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = the minimal m such that ∏_{k=0}^{m-1} (n-k)/n ≤ 1/2 (i.e.
 *            probability of a shared birthday ≥ ½)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n must be at least 1; n == 0 is undefined – return 0. */
    if (n == 0) return 0ULL;

    /* Special‑case the very first simple situations */
    if (n == 1 || n == 2) return 2ULL;

    /* Pre‑compute constants in long double, which is safe for all n up
       to 2^64.  The calculation uses only floating point for the
       comparison; seq_t arithmetic stays integer. */
    const long double log_n   = logl((long double)n);
    const long double log_half = -logl(2.0L);          /* log(1/2) */

    long double logP = 0.0L;                         /* log( product / n^m ) */
    for (idx_t m = 1; ; ++m)
    {
        /* Add log((n-m+1)/n) to logP.  For m>n the factor becomes 0,
           which would make log(0) undefined – but for n ≥ 3 this
           never occurs before the condition is met. */
        long double term = logl((long double)(n - m + 1));
        logP += term - log_n;

        if (logP <= log_half)                       /* product ≤ ½ */
            return (seq_t)m;
    }
}
