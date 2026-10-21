/*
 * A216506  Least number k such that k*n+1 is a prime dividing n^(2n) - 1.
 * https://oeis.org/A216506
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that (k*n + 1) is prime and divides
 *            n^(2n) - 1
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 18, u64 n <= 670,
 *            u128 n <= 670, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined for n < 2 */

    for (idx_t k = 1;; ++k)
    {
        seq_t p = (seq_t)k * (seq_t)n + 1;
        if (!is_prime_big(p))
            continue;

        /* Test whether p | n^(2n) - 1  <=>  n^(2n) ≡ 1 (mod p) */
        if (mod_pow((seq_t)n, (uint64_t)(2 * n), p) == 1)
            return (seq_t)k;
    }
}
