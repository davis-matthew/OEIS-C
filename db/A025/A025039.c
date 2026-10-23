/*
 * A025039  Number of partitions of { 1, 2, ..., 7n } into sets of size 7.
 * https://oeis.org/A025039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7n)! / (n! * (7!)^n) (OEIS A025039) Computed by
 *            multiplicative recurrence: a(1) = 1 a(n) = a(n-1) *
 *            (7n)(7n-1)...(7n-6) / (n * 7!)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, return 0 for n<1 */

    const seq_t fact7 = (seq_t)5040;   /* 7! */

    seq_t res = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute product (7k)(7k-1)...(7k-6) */
        seq_t prod = (seq_t)1;
        seq_t base = (seq_t)7 * (seq_t)k;          /* 7k as seq_t */
        for (int i = 0; i < 7; ++i)
            prod = prod * (base - (seq_t)i);

        /* multiply current result by prod, then divide by k*7! */
        /* Division is exact by construction */
        seq_t denom = (seq_t)k * fact7;
        res = (res * prod) / denom;
    }

    return res;
}
