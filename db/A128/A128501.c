/*
 * A128501  a(n) = lcm{1 <= k <= n, gcd(k, 3) = 1}.
 * https://oeis.org/A128501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm{1 <= k <= n, gcd(k,3)=1}
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements by multiplying the highest power of each prime p≠3
   not exceeding n: product over p≤n, p≠3 of p^⌊log_p n⌋. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n < 2)
        return 1;                  /* a(0) = a(1) = 1 */

    /* Sieve of Eratosthenes to generate primes up to n */
    size_t limit = (size_t)n;
    bool *is_composite = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_composite)
        return 0;                  /* allocation failure: unlikely in tests */

    for (size_t i = 0; i <= limit; ++i)
        is_composite[i] = false;

    for (size_t i = 2; (size_t)i * i <= limit; ++i)
    {
        if (!is_composite[i])
        {
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    seq_t result = 1;
    uint64_t n64 = n;               /* safety: n fits in uint64_t */

    for (size_t i = 2; i <= limit; ++i)
    {
        if (is_composite[i])
            continue;              /* not a prime */

        if (i == 3)
            continue;              /* skip prime 3 */

        /* Compute the largest power of i that does not exceed n */
        uint64_t p_pow = i;
        while ((uint64_t)p_pow * (uint64_t)i <= n64)
            p_pow *= i;

        result *= (seq_t)p_pow;
    }

    free(is_composite);
    return result;
}
