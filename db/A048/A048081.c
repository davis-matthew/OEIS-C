/*
 * A048081  Number of nonempty subsets of {1,2,...,n} in which exactly 5/6 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m>=1, 5m <= floor((n+2)/3), m <= n - floor((n+2)/3)}
 *            of C(floor((n+2)/3), 5m) * C(n - floor((n+2)/3), m) (OEIS
 *            A048081)
 * Range:     u8 n <= 19, u16 n <= 36, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* threshold L = floor((n+2)/3) */
    idx_t L = (n + 2) / 3;

    /* bound on m: 5m <= L  and  m <= n - L  */
    idx_t limit = L / 5;             /* ensures 5m <= L   */
    idx_t remaining = n - L;         /* largest m from high part */
    if (remaining < limit) limit = remaining;

    seq_t sum = 0;
    for (idx_t m = 1; m <= limit; ++m) {
        idx_t five_m = 5 * m;              /* safe: m <= L/5, L < n */
        seq_t low  = binom(L, five_m);     /* choose from {1..L}   */
        seq_t high = binom(n - L, m);      /* choose from {L+1..n} */
        sum += low * high;
    }
    return sum;
}
