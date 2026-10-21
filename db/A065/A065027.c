/*
 * A065027  a(n) is the smallest k > 0 such that n^k < k!.
 * https://oeis.org/A065027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the smallest k > 0 such that n^k < k!
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* special case n = 0: 0^k = 0 < k! for any k >= 1, so a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* logarithm of n, used in the inequality test */
    long double ln_n = logl((long double)n);
    long double log_fact = 0.0L;   /* running sum log(1)+log(2)+... */

    /* we search for the smallest k such that  k * ln(n) < log(k!)  */
    /* the loop is guaranteed to terminate because k grows without bound
       while log(k!) eventually dominates k*ln(n)  */
    for (idx_t k = 1; ; ++k) {
        log_fact += logl((long double)k);
        /* use a tiny epsilon to avoid FP rounding causing an off-by-one */
        if ((long double)k * ln_n < log_fact - 1e-12L)
            return (seq_t)k;
    }
}
