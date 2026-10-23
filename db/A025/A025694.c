/*
 * A025694  Index of 2^n within the sequence of the numbers of the form 2^i *
 *          9^j.
 * https://oeis.org/A025694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=1..n} ceil(k * log_9(2))
 * Range:     u8 n <= 38, u16 n <= 642, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where log_9(2) = 1 / log2(9) */
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    static long double inv_log2_9 = 0.0L;
    if (inv_log2_9 == 0.0L) {
        /* log2(9) = log(9) / log(2) */
        long double log2_9 = logl(9.0L) / logl(2.0L);
        inv_log2_9 = 1.0L / log2_9;  /* log_9(2) */
    }

    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)1;      /* a(0) */
    long double acc   = 0.0L;
    const long double eps = 1e-12L;

    for (idx_t k = 1; k <= n; ++k) {
        acc += inv_log2_9;
        idx_t term = (idx_t)acc;          /* floor of acc */
        if (acc - term > eps) term++;     /* convert to ceil with a safety margin */
        sum += (seq_t)term;
    }

    return sum;
}
