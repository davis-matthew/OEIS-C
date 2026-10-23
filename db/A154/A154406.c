/*
 * A154406  Larger twin primes in A061237.
 * https://oeis.org/A154406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p such that p % 18 == 1 and p-2 is also prime
 *            (i.e., the larger member of a twin prime pair in A061237)
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; behaviour below offset irrelevant */

    seq_t p = 19;                          /* first prime ≡ 1 (mod 18) that is the larger twin */
    idx_t count = 0;                       /* how many valid primes found so far */

    while (1) {
        if (is_prime_big(p) && is_prime_big(p - 2)) {
            ++count;
            if (count == n) return p;
        }
        p += 18;                           /* next candidate of the form 18k+1 */
    }
}
