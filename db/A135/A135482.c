/*
 * A135482  a(n) = (1/4)*Sum_{j=1..n} 2^prime(j).
 * https://oeis.org/A135482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4)*Sum_{j=1..n} 2^prime(j)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 0;
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j)
    {
        uint32_t p = get_prime(j);
        sum += pow2((idx_t)p);
    }
    return sum >> 2u;
}
