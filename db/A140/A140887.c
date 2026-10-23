/*
 * A140887  Number of nonprimes in [30n - 30, 30n] coprime to 30.
 * https://oeis.org/A140887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = |{x ∈ [30(n−1),30n] : gcd(x,30)=1 and x is not prime}|
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, but guard against n=0 */

    /* start of the block [30(n−1), 30n] */
    idx_t base = (n - 1) * 30;

    /* offsets of the numbers coprime to 30 within each block */
    static const idx_t offs[8] = {1, 7, 11, 13, 17, 19, 23, 29};

    seq_t nonprimes = 0;                   /* count of composite/1 numbers */

    for (int i = 0; i < 8; ++i) {
        idx_t val = base + offs[i];
        if (!is_prime(val))
            ++nonprimes;
    }

    return nonprimes;
}
