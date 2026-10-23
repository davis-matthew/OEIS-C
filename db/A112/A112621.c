/*
 * A112621  If p^b(p,n) is the highest power of the prime p dividing n, then
 *          a(n) = sum_{p|n} b(p,n)^b(p,n).
 * https://oeis.org/A112621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p|n} b(p,n)^b(p,n) (sum over distinct prime
 *            divisors)
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ipow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_uint64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0; /* a(1) = 0 (empty sum) */

    uint64_t tmp = (uint64_t)n;
    seq_t     sum = 0;

    /* factor out 2 */
    uint64_t exp = 0;
    while ((tmp & 1ULL) == 0) {
        ++exp;
        tmp >>= 1;
    }
    if (exp > 0)
        sum += ipow_uint64(exp, exp);

    /* trial division by odd numbers */
    for (uint64_t i = 3; i <= tmp / i; i += 2U) {
        if (tmp % i == 0) {
            exp = 0;
            while (tmp % i == 0) {
                ++exp;
                tmp /= i;
            }
            sum += ipow_uint64(exp, exp);
        }
    }

    /* any remaining prime factor has exponent 1 */
    if (tmp > 1)
        sum += (seq_t)1;  /* 1^1 = 1 */

    return sum;
}
