/*
 * A167021  a(n) = 1 iff 6n+1 is prime.
 * https://oeis.org/A167021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff 6n+1 is prime, otherwise 0
 * Range:     u8 n <= 42, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t first, then compute the candidate. */
    seq_t p = (seq_t)6 * (seq_t)n + (seq_t)1;
    return is_prime_big(p) ? (seq_t)1 : (seq_t)0;
}
