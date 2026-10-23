/*
 * A173481  a(n) = the smallest number ending in n zeros divisible by n.
 * https://oeis.org/A173481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = lcm(n, 10^n) = n * 2^(n - v2(n)) * 5^(n - v5(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   _pow_by_squaring
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v2(n) and v5(n) are the exponents of 2 and 5 in the prime
   factorisation of n. */

#include <stdint.h>
#include "../utils/_pow_by_squaring.h"

seq_t a(idx_t n)
{
    /* The problem specification guarantees n >= 1. */
    idx_t v2 = 0, temp = n;
    while ((temp & 1ULL) == 0ULL && temp > 0ULL)
    {
        ++v2;
        temp >>= 1;
    }

    idx_t v5 = 0;
    temp = n;
    while (temp % 5ULL == 0ULL && temp > 0ULL)
    {
        ++v5;
        temp /= 5ULL;
    }

    /* Compute n * 2^(n-v2) * 5^(n-v5) using fast exponentiation. */
    seq_t result = (seq_t)n;
    result *= _pow_by_squaring((seq_t)2, n - v2);
    result *= _pow_by_squaring((seq_t)5, n - v5);
    return result;
}
