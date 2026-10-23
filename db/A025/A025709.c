/*
 * A025709  Index of 5^n within the sequence of the numbers of the form
 *          5^i*8^j.
 * https://oeis.org/A025709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=1..n} ceil(k * log_8(5))
 * Range:     u8 n <= 24, u16 n <= 410, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static const long double C_LOG = logl(5.0L) / logl(8.0L);

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* compute the running sum of ceil(k * c) */
    seq_t sum = 1;                       /* a(0) */
    for (idx_t k = 1; k <= n; ++k) {
        long double prod = (long double)k * C_LOG;
        idx_t inc = (idx_t)ceill(prod);  /* ceil(k * c) */
        sum += inc;
    }
    return sum;
}
