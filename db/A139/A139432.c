/*
 * A139432  a(0)=0. For n>=1, a(n) = a(n-1) - d(n) if a(n-1) >= d(n); a(n) =
 *          a(n-1) + d(n) if a(n-1) < d(n); where d(n) is the number of
 *          positive divisors of n.
 * https://oeis.org/A139432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0; for n>=1: a(n)=a(n-1)-d(n) if a(n-1)>=d(n); otherwise a(n)=a(n-1)+d(n) */
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) {
        seq_t zero = 0;
        return zero;
    }

    seq_t prev = 0;   /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t d = divisor_count_u64(i);
        seq_t di = (seq_t)d;

        /* Update according to the recurrence */
        if (prev >= di)
            prev = prev - di;
        else
            prev = prev + di;
    }
    return prev;
}
