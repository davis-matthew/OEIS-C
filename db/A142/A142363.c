/*
 * A142363  Primes congruent to 12 mod 47.
 * https://oeis.org/A142363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 12 modulo 47
 * Range:     u8 n <= 1, u16 n <= 144, u32 n <= 99, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; 0 is unused */

    seq_t candidate = 59;          /* 47*1 + 12, the first such prime */
    if (n == 1) return candidate;

    idx_t count = 1;
    /* iterate over numbers congruent to 12 (mod 47) and test primality */
    for (candidate += 47; ; candidate += 47)
    {
        if (is_prime_big(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
