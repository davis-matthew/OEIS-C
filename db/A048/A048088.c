/*
 * A048088  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..floor(n/5)} C(floor((n+3)/3), 2k) * C(n -
 *            floor((n+3)/3), 3k)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum over all subset sizes m=5k where 2k <= floor((n+3)/3) and 3k <= n - floor((n+3)/3). */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    idx_t A = (n + 3) / 3;          /* floor((n+3)/3) – elements <= threshold */
    idx_t B = n - A;                /* elements > threshold           */

    idx_t maxK = n / 5;
    idx_t limit1 = A / 2;           /* 2k <= A  -> k <= A/2          */
    if (maxK > limit1) maxK = limit1;
    idx_t limit2 = B / 3;           /* 3k <= B  -> k <= B/3          */
    if (maxK > limit2) maxK = limit2;

    seq_t result = 0;
    for (idx_t k = 1; k <= maxK; ++k) {
        seq_t c1 = binom(A, 2 * k);
        seq_t c2 = binom(B, 3 * k);
        result += c1 * c2;
    }
    return result;
}
