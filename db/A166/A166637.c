/*
 * A166637  Totally multiplicative sequence with a(p) = 7*(p-1) for prime p.
 * https://oeis.org/A166637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e∥n} (7·(p−1))^e
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 4095, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset is 1, n==0 is unused */
    if (n == 1)
        return (seq_t)1;            /* a(1) = 1 */

    uint64_t temp = (uint64_t)n;     /* work on a 64‑bit copy */
    seq_t result = (seq_t)1;

    while (temp > 1)
    {
        uint64_t p = smallest_prime_factor(temp);   /* prime divisor */
        uint64_t exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            exp++;
        }

        seq_t base = (seq_t)7 * ((seq_t)p - (seq_t)1);
        seq_t pow = (seq_t)1;
        for (uint64_t i = 0; i < exp; ++i)
            pow *= base;

        result *= pow;
    }

    return result;
}
