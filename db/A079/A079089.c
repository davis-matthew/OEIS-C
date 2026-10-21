/*
 * A079089  Sum of divisors of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma( (prime(n)+1)*(prime(n+1)+1)/4 )
 * Range:     u8 n <= 7, u16 n <= 62, u32 n <= 6311, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided utilities:
   - get_prime(idx_t k): returns the k‑th prime (uint32_t)
   - sigma_of(idx_t n): returns the sum of divisors of n (seq_t)
*/
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for offset 0, return 0 */
        return 0;
    uint32_t p1 = get_prime(n);     /* nth prime */
    uint32_t p2 = get_prime(n + 1); /* (n+1)th prime */

    /* (p1+1)*(p2+1) is divisible by 4 for all n >= 1 */
    uint64_t prod = (uint64_t)(p1 + 1) * (uint64_t)(p2 + 1);
    uint64_t x    = prod / 4;          /* the number to take sigma of */

    return sigma_of((idx_t)x);
}
