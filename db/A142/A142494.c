/*
 * A142494  Primes congruent to 29 mod 51.
 * https://oeis.org/A142494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 29 mod 51
 * Range:     u8 n <= 3, u16 n <= 189, u32 n <= 190, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; behaviour below offset does not matter */
        return 0;

    if (n == 1)                 /* first term is 29 */
        return 29;

    idx_t      count = 1;       /* we already have 29 as the first prime */
    seq_t      cand   = 29;

    while (count < n) {
        cand += 51;             /* next candidate congruent to 29 mod 51 */
        if (is_prime_big(cand))
            ++count;
    }
    return cand;
}
