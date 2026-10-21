/*
 * A068186  a(n) is the largest number whose product of decimal digits equals
 *          n^n.
 * https://oeis.org/A068186
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the largest number whose decimal digit product equals
 *            n^n. * It is obtained by concatenating the prime factors of n^n
 *            in descending * prime order (7,5,3,2), each repeated e_i·n
 *            times, where e_i is the * exponent of prime p_i in n. If n has a
 *            prime factor >7 the value is 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"

static seq_t big_block(idx_t n, int prime, idx_t ep)
{
    if (ep == 0) return 0;
    idx_t k = ep * n;                      /* exponent in n^n          */
    seq_t pw = pow10(k);                    /* 10^k                     */
    seq_t block = (pw - 1) * (seq_t)prime;  /* prime*(10^k-1)           */
    block = block / 9;                      /* (prime*(10^k-1))/9       */
    return block;
}

seq_t a(idx_t n)
{
    if (n < 2u) return 0;          /* unspecified offset handling */

    /* factor n into primes 2,3,5,7 */
    idx_t m = n;
    idx_t e2 = 0, e3 = 0, e5 = 0, e7 = 0;

    while ((m & 1u) == 0u) { e2++; m >>= 1u; }          /* divide by 2 */
    while (m % 3u == 0u) { e3++; m /= 3u; }
    while (m % 5u == 0u) { e5++; m /= 5u; }
    while (m % 7u == 0u) { e7++; m /= 7u; }

    if (m > 1u) return 0;          /* contains a prime > 7 */

    seq_t result = 0;

    /* helper to append a block of a given prime */
    result = result * pow10(e7 * n) + big_block(n, 7, e7);
    result = result * pow10(e5 * n) + big_block(n, 5, e5);
    result = result * pow10(e3 * n) + big_block(n, 3, e3);
    result = result * pow10(e2 * n) + big_block(n, 2, e2);

    return result;
}
