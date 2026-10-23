/*
 * A138427  a(n) = (prime(n)^5 - prime(n))/6.
 * https://oeis.org/A138427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^5 - prime(n))/6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1u)
        return 0; /* undefined for n < 1, return 0 */

    uint32_t pk = get_prime(n);   /* primes are 1‑based: prime(1)=2 */

    seq_t p   = (seq_t)pk;
    seq_t p2  = p * p;
    seq_t p4  = p2 * p2;
    seq_t pow5 = p4 * p;

    seq_t res = (pow5 - p) / 6;
    return res;
}
