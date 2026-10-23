/*
 * A140366  a(n) = isprime(6n-1) + 2*isprime(6n-5), where isprime = A010051.
 * https://oeis.org/A140366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010051(6*n-1) + 2*A010051(6*n-5)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t v1 = 6 * n - 1;   /* 6n-1 */
    idx_t v2 = 6 * n - 5;   /* 6n-5 */
    seq_t res = 0;
    if (is_prime(v1)) res += 1;
    if (is_prime(v2)) res += 2;
    return res;
}
