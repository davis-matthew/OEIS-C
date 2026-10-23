/*
 * A024524  4th elementary symmetric function of {1, p(1), p(2), ..., p(n-1)},
 *          where p(0) = 1.
 * https://oeis.org/A024524
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the 4th elementary symmetric sum of * {1, p(1), p(2), …,
 *            p(n-1)}, where p(0)=1. * i.e. a(n) = Σ_{1≤i<j<k<ℓ≤n-1}
 *            p(i)p(j)p(k)p(ℓ), with p(1)=2, p(2)=3, … * For n < 4 the value
 *            is undefined in OEIS → this implementation returns 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;                       /* undefined before offset          */

    seq_t e1 = 1;    /* elementary sums for 1-element subsets: 1 = p(0)       */
    seq_t e2 = 0;    /* sums of products of 2 distinct elements               */
    seq_t e3 = 0;    /* sums of products of 3 distinct elements               */
    seq_t e4 = 0;    /* target sum of products of 4 distinct elements         */

    for (idx_t k = 1; k <= n - 1; ++k) {      /* iterate over p(1)..p(n-1)      */
        seq_t x = get_prime(k);                /* p(k)                             */
        e4 += x * e3;                          /* new 4‑term products              */
        e3 += x * e2;                          /* new 3‑term products              */
        e2 += x * e1;                          /* new 2‑term products              */
        e1 += x;                               /* update 1‑term sum                */
    }

    return e4;                                 /* final 4th elementary sum        */
}
