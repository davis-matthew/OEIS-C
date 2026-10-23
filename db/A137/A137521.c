/*
 * A137521  Prime numbers concatenated with 45.
 * https://oeis.org/A137521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = prime(n) concatenated with 45
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* Undefined for n < 1, return 0 */

    /* Retrieve the nth prime (1‑indexed) */
    uint32_t p = get_prime(n);

    /* Concatenate with 45 by multiplying by 100 and adding 45 */
    seq_t result = ((seq_t)p) * 100;
    result += 45;

    return result;
}
