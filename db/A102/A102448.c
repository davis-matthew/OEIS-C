/*
 * A102448  a(n) is the number of ways to write n = k^2 * j, j <= k, gcd(k,j)
 *          = 1, where j and k are positive integers.
 * https://oeis.org/A102448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (k, j) | n = k^2 * j, j <= k, gcd(k, j) = 1 }
 * Range:     u8 n <= 9659, u16 n <= 9659, u32 n <= 9659, u64 n <= 9659,
 *            u128 n <= 9659, big any n
 * Helpers:   gcd_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_local.h"

/* Local gcd for seq_t values – helper function, not part of the provided list */

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* iterate k such that k^2 <= n */
    for (idx_t k = 1; k <= n / k; ++k) {
        idx_t k2 = k * k;                     /* k^2 fits in idx_t */
        if (n % k2 != 0)
            continue;
        idx_t j = n / k2;                     /* remaining factor */
        if (j > k)
            continue;
        if (gcd_local((seq_t)k, (seq_t)j) == 1)
            ++count;
    }
    return count;
}
