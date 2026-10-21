/*
 * A078589  a(1)=0, a(2)=1, a(n) = abs(abs(a(n-1)) - a(n-2) - n + 1).
 * https://oeis.org/A078589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, a(n) = | |a(n-1)| – a(n-2) – n + 1 | (for n ≥ 3)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) for any n ≥ 1.
   The computation uses only integer arithmetic; 
   intermediate values are kept in unsigned __int128 to avoid overflow. */
seq_t a(idx_t n)
{
    if (n == 0) abort();                 /* offset is 1, n must be ≥ 1 */

    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* a(1) and a(2) are the starting values */
    seq_t prev2 = (seq_t)0;              /* a(n-2) */
    seq_t prev1 = (seq_t)1;              /* a(n-1) */
    seq_t cur   = 0;                     /* a(n) placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        /* sum = a(n-2) + n - 1  */
        unsigned __int128 sum = (unsigned __int128)prev2
                               + (unsigned __int128)i
                               - (unsigned __int128)1;

        unsigned __int128 uprev1 = (unsigned __int128)prev1;
        unsigned __int128 diff;

        /* diff = |prev1 - sum| */
        if (uprev1 > sum)
            diff = uprev1 - sum;
        else
            diff = sum - uprev1;

        cur = (seq_t)diff;          /* a(i) */

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
