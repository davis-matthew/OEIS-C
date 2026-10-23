/*
 * A142689  Primes congruent to 40 mod 57.
 * https://oeis.org/A142689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 40 (mod 57)
 * Range:     u8 n <= 2, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"   /* is_prime(idx_t) */

/* Return the n‑th prime that is congruent to 40 modulo 57.
   The sequence is indexed from 1 (a(1) = 97). */
seq_t a(idx_t n)
{
    if (n == 0)   /* safety for unexpected 0 input */
        return (seq_t)0;

    /* first prime in the progression 40 mod 57 is 97 */
    idx_t candidate = 97;
    idx_t count     = 0;

    for (;;) {
        /* test current candidate for primality */
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        /* move to the next number in the residue class 40 mod 57 */
        candidate += (idx_t)57;
    }
}
