/*
 * A130771  Numbers n not divisible by 3 for which neither 2n-3 nor 2n+3 are
 *          primes.
 * https://oeis.org/A130771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th integer not divisible by 3 such that both 2*n-3
 *            * and 2*n+3 are composite. The function searches sequentially
 * Range:     u8 n <= 28, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 1;

    while (count < n) {
        if (m % 3 == 0) {          /* skip multiples of 3 */
            ++m;
            continue;
        }

        int64_t z1 = 2 * (int64_t)m - 3;  /* 2*n-3 */
        bool p1 = false;
        if (z1 >= 2)                /* only check if >= 2 */
            p1 = is_prime((idx_t)z1);

        int64_t z2 = 2 * (int64_t)m + 3;  /* 2*n+3 */
        bool p2 = is_prime((idx_t)z2);

        if (!p1 && !p2) {            /* both composite */
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }

    return 0;   /* never reached */
}
