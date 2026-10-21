/*
 * A085972  Number of numbers <= n that are primes or not prime powers.
 * https://oeis.org/A085972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 - #{ prime powers p^k ≤ n with k ≥ 2 }
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (i.e. count 2..n and subtract composite prime powers) */

#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    /* sum of composite prime powers ≤ n */
    seq_t comp = 0;

    /* only primes ≤ √n can have powers ≤ n */
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (!isPrime(p))
            continue;

        seq_t val = (seq_t)p * (seq_t)p;          /* start with p² */
        while (val <= n)
        {
            ++comp;
            /* next power, careful with overflow */
            if (val > n / p)          /* next multiplication would exceed n */
                break;
            val *= p;
        }
    }

    /* a(n) = (n-1) - comp, n≥1 */
    return (seq_t)n - 1 - comp;
}
