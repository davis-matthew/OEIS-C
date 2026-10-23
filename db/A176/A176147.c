/*
 * A176147  a(n) = n^sopf(n), where sopf(n) is the sum of the distinct primes
 *          dividing n (A008472).
 * https://oeis.org/A176147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^sopf(n) * where sopf(n) is the sum of the distinct
 *            primes dividing n (A008472)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the sum of distinct prime factors of n. */
    uint64_t tmp = (uint64_t)n;
    uint64_t sum = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            sum += p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)
        sum += tmp;               /* remaining prime factor */

    /* Compute n^sum using the arbitrary‑precision power routine. */
    return big_pow((seq_t)n, (seq_t)sum);
}
