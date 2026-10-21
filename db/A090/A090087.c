/*
 * A090087  Smallest odd pseudoprimes to base n, not necessarily exceeding n.
 *          Compare with A007535 and A090086.
 * https://oeis.org/A090087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest odd composite pseudoprime to base n (i.e.,
 *            the minimal odd composite x such that n^(x-1) ≡ 1 (mod x))
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;

    for (idx_t x = 3; ; x += 2)            /* search odd numbers only   */
    {
        if (is_prime_small((seq_t)x))     /* skip primes – they are not pseudoprimes   */
            continue;

        /* compute n^(x-1) mod x                     */
        seq_t r = mod_pow(base, (uint64_t)(x - 1), (seq_t)x);

        if (r == 1)                       /* n^(x-1) ≡ 1 (mod x)  */
            return (seq_t)x;
    }
}
