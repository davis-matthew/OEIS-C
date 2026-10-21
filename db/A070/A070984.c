/*
 * A070984  a(n) = k such that (k-1)*log(k-1) < n < k*log(k).
 * https://oeis.org/A070984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = smallest k with (k-1)*log(k-1) < n < k*log(k) */
#include <math.h>

seq_t a(idx_t n)
{
    uint64_t k = 2;                     /* k starts at 2   */
    double dn = (double)n;              /* n as double     */

    while (1) {
        double left  = (double)(k - 1) * log((double)(k - 1));
        double right = (double)k * log((double)k);

        if (dn > left && dn < right)
            return (seq_t)k;            /* found the correct k */
        ++k;
    }
}
