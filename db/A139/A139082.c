/*
 * A139082  a(n) = (largest power of a prime dividing n) * (largest power of a
 *          prime dividing (n+1)).
 * https://oeis.org/A139082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034699(n) * A034699(n+1) where A034699(k) is the largest
 *            prime power dividing k
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_power.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined at offset 0; safe guard */
        return (seq_t)0;

    seq_t lp1 = largest_prime_power((uint64_t)n);
    seq_t lp2 = largest_prime_power((uint64_t)(n + 1));

    return lp1 * lp2;
}
