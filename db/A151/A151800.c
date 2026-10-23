/*
 * A151800  Least prime > n (version 2 of the "next prime" function).
 * https://oeis.org/A151800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = least prime > n, i.e., A007918(n+1)
 * Range:     u8 n <= 250, u16 n <= 9962, u32 n <= 9962, u64 n <= 9962,
 *            u128 n <= 9962, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Convert index to sequence type. */
    seq_t m = (seq_t)n + 1;

    /* For n = 0 the next prime is 2. */
    if (m < 2) return 2;

    /* If n + 1 is itself prime (including 2) return it. */
    if (m == 2) return 2;

    /* All primes > 2 are odd – if m is even start from the next odd. */
    if ((m & 1) == 0) m += 1;

    /* Increment by 2 (odd numbers only) until a prime is found. */
    while (!is_prime_big(m))
        m += 2;

    return m;
}
