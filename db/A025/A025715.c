/*
 * A025715  Index of 6^n within the sequence of the numbers of the form
 *          5^i*6^j (A025622).
 * https://oeis.org/A025715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=1..n} ceil(k * log_5(6))
 * Range:     u8 n <= 20, u16 n <= 342, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static const double LOG6 = log(6.0);
static const double LOG5 = log(5.0);
static const double LOG5_6 = LOG6 / LOG5;          /* log_5(6) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                          /* a(0) = 1 */

    seq_t sum = (seq_t)1;                       /* start with 1 for a(0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute ceil(k * log_5(6)) using double arithmetic */
        uint64_t add = (uint64_t)ceill((long double)k * LOG5_6);
        sum += (seq_t)add;
    }

    return sum;                                  /* a(n) */
}
