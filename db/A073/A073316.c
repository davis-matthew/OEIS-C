/*
 * A073316  a(n) = Max d(j), j=1..n-1, where d(j) is the smallest positive
 *          number such that 2j+d(j) and 2n+d(j) are both prime. A
 *          generalization of A073310.
 * https://oeis.org/A073316
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max{ d(j) | 1 ≤ j < n, d(j) = min{ d>0 | 2*j+d and 2*n+d
 *            are prime } }
 * Range:     u8 n <= 933, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* compute a( n ) for n >= 2.
   The function returns 0 for n < 2, which is irrelevant
   because the sequence is defined for n >= 2. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    idx_t dmax = 0;  /* store maximum d(j) found for j < n */

    for (idx_t j = 1; j < n; ++j) {
        idx_t d = 1;  /* d must be odd; start with 1 */

        /* find the smallest d such that both 2*j+d and 2*n+d are prime */
        while (true) {
            idx_t v1 = 2 * j + d;
            idx_t v2 = 2 * n + d;

            if (is_prime(v1) && is_prime(v2))
                break;

            d += 2;   /* next odd candidate */
        }

        if (d > dmax) dmax = d;
    }

    /* cast to seq_t (the output type). The value fits for the
       tested range; if seq_t is smaller, overflow is as per spec. */
    return (seq_t)dmax;
}
