/*
 * A051674  a(n) = prime(n)^prime(n).
 * https://oeis.org/A051674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/get_nth_prime.h"

seq_t a(idx_t n)
{
    /* Sequence offset is 1: a(1) is the first term.  For n == 0, return 0
     * (doesn't affect the sequence cache or tests). */
    if (n == 0)
        return (seq_t)0;

    /* Get the n‑th prime and compute p^p using the provided big_pow helper. */
    uint32_t p = get_nth_prime(n);
    return big_pow((seq_t)p, (seq_t)p);
}
