/*
 * A179931  a(n) = gcd(sigma(n), sigma_2(n)).
 * https://oeis.org/A179931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), sigma_2(n))
 * Range:     u8 n <= 15, u16 n <= 873, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma_k(m) is the sum of the k-th powers of the divisors of m */

#include <stdint.h>
#include "../utils/sigma_k.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* the answer is defined for n >= 1; for n == 0 return 0 */
    if (n == 0) return 0;

    /* compute the sum of divisors and the sum of squared divisors */
    seq_t sum1 = sigma_k(n, 1);
    seq_t sum2 = sigma_k(n, 2);

    /* return their greatest common divisor */
    return gcd(sum1, sum2);
}
