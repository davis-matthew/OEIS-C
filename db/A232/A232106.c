/*
 * A232106  Number of groups of order prime(n)^6.
 * https://oeis.org/A232106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of groups of order prime(n)^6. For a prime p > 3:
 *            a(n) = 3p² + 39p + 344 + 24 gcd(p-1,3) + 11 gcd(p-1,4) + 2
 *            gcd(p-1,5). Special cases: a(1)=267 (for p=2), a(2)=504 (for
 *            p=3)
 * Range:     u8 none, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the offset and special cases for p = 2 and 3 */
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */
    if (n == 1) return (seq_t)267;  /* p = 2 */
    if (n == 2) return (seq_t)504;  /* p = 3 */

    /* Retrieve the nth prime */
    seq_t p = (seq_t)get_prime(n); /* get_prime returns uint32_t */

    /* Compute the formula for p > 3 */
    seq_t p2 = p * p;
    seq_t res = 3 * p2 + 39 * p + 344;

    /* GCD terms with small constants 3, 4, 5 */
    seq_t d3 = gcd(p - (seq_t)1, (seq_t)3);
    seq_t d4 = gcd(p - (seq_t)1, (seq_t)4);
    seq_t d5 = gcd(p - (seq_t)1, (seq_t)5);

    res += 24 * d3 + 11 * d4 + 2 * d5;
    return res;
}
