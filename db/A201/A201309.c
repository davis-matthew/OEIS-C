/*
 * A201309  Primes of the form k^3 - 4.
 * https://oeis.org/A201309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: primes of the form k^3 – 4, for integer k ≥ 3.  The
   sequence is ordered by increasing value of k.  The first prime
   is 23 (k = 3). */
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

seq_t a(idx_t n)
{
    /* n is at least 1; behaviour for n < 1 is undefined. */
    uint64_t count = 0;      /* number of primes found so far */
    for (uint64_t k = 3; ; k += 2)     /* only odd k can give an odd result */
    {
        uint64_t cand = k * k * k - 4;  /* k^3 – 4, fits in 64 bits for k ≤ ~1e6 */
        if (is_prime(cand))
        {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
