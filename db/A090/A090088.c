/*
 * A090088  Smallest even pseudoprimes to odd base=2n-1, not necessarily
 *          exceeding n. See also A007535 and A090086, A090087.
 * https://oeis.org/A090088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest even composite x > 2 such that (2n-1)^(x-1) ≡ 1
 *            (mod x)
 * Range:     u8 n <= 1, u16 n <= 9443, u32 n <= 9443, u64 n <= 9443,
 *            u128 n <= 9443, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Base B = 2n-1 (odd) */
    seq_t B = (seq_t)(2 * (seq_t)n - 1);

    /* Start from the smallest even candidate above 2. */
    seq_t x = 4;
    while (1)
    {
        /* test whether B^(x-1) ≡ 1 (mod x) */
        if (mod_pow(B, (idx_t)(x - 1), x) == (seq_t)1)
            return x;               /* found the minimal even pseudoprime */

        /* move to the next even number */
        x += 2;
    }
}
