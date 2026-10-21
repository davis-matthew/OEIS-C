/*
 * A065704  Number of squares or twice squares dividing n.
 * https://oeis.org/A065704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   multiplicative product of (e+1) for 2^e and floor(e/2)+1 for odd
 *            prime powers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;
    int exp = 0;

    /* factor out powers of 2 */
    while ((temp & 1) == 0) {          /* temp % 2 == 0 */
        exp++;
        temp >>= 1;                    /* temp /= 2 */
    }
    if (exp > 0)
        result *= (seq_t)(exp + 1);

    /* factor odd primes by trial division */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                exp++;
                temp /= p;
            }
            /* odd prime power */
            result *= (seq_t)(exp / 2 + 1);
        }
    }

    /* remaining prime factor (if any) */
    if (temp > 1)
        result *= (seq_t)(1);  /* exponent 1 -> floor(1/2)+1 = 1 */

    return result;
}
