/*
 * A076930  Smallest k such that n*k is an n-th power.
 * https://oeis.org/A076930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007947(n)^n / n, where A007947(n) is the least m such
 *            that n | m^n (i.e., the product of the distinct prime divisors
 *            of n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    idx_t temp = n;
    seq_t result = 1;

    /* factor out the prime 2 */
    idx_t cnt = 0;
    while ((temp & 1) == 0) {      /* temp % 2 == 0 */
        temp >>= 1;                /* divide by 2 */
        cnt++;
    }
    if (cnt)
        result *= pow_int((seq_t)2, n - cnt);

    /* factor odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                cnt++;
            }
            result *= pow_int(p, n - cnt);
        }
    }

    /* if any prime factor remains */
    if (temp > 1)
        result *= pow_int(temp, n - 1);

    return result;
}
