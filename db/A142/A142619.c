/*
 * A142619  Primes congruent to 26 mod 55.
 * https://oeis.org/A142619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime which is congruent to 26 modulo 55
 * Range:     u8 n <= 1, u16 n <= 128, u32 n <= 144, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the n-th prime of the form 55*k+26,  with a(1)=191. */
seq_t a(idx_t n)
{
    seq_t candidate = 26;   /* first possible value of the form 55*k+26 */
    idx_t count = 0;        /* number of such primes found so far */

    while (count < n) {
        if (is_prime_big(candidate)) {
            count++;
            if (count == n)
                return candidate;
        }
        candidate += 55;   /* move to next residue 26 mod 55 */
    }
    /* unreachable, but keep compiler happy */
    return candidate;
}
