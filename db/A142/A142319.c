/*
 * A142319  Primes congruent to 14 mod 45.
 * https://oeis.org/A142319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 281, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime congruent to 14 (mod 45) */
/* The function searches in the arithmetic progression 14 + 45k
   and returns the nth prime encountered. It works for any
   integer type supplied for seq_t, idx_t, and uses is_prime
   from ../utils/is_prime.h for primality testing. */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behaviour below offset does not matter */

    uint64_t cand = 14;                 /* start of the progression */
    idx_t    count = 0;                 /* number of primes found so far */

    for (;;) {
        cand += 45;                     /* next term in the progression */
        if (is_prime((idx_t)cand)) {    /* primality test */
            ++count;
            if (count == n)
                return (seq_t)cand;     /* found the desired prime */
        }
    }
}
