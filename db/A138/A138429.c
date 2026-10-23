/*
 * A138429  a(n) = (prime(n)^5 - prime(n))/15.
 * https://oeis.org/A138429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^5 - prime(n))/15
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1. If called with n == 0,
       return 0 to avoid undefined behavior. */
    if (n == 0)
        return (seq_t)0;

    /* nth prime (1‑based).  get_prime returns a uint32_t. */
    uint32_t p32 = get_prime(n);

    /* Promote to seq_t for arithmetic. */
    seq_t p = (seq_t)p32;

    /* Compute p^5.  Using big_pow guarantees exactness for
       arbitrary‑precision seq_t and works correctly on small
       integer types as well. */
    seq_t p5 = big_pow(p, (seq_t)5);

    /* Compute (p5 - p) / 15.  All operations are in seq_t. */
    seq_t result = (p5 - p) / (seq_t)15;

    return result;
}
