/*
 * A201173  Primes of the form 5n^3+3.
 * https://oeis.org/A201173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime of the form 5*k^3+3, starting with k = 0.
   The function iterates over successive integers k, checks whether
   5*k^3+3 is prime, and returns the value when the n-th prime is found. */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; behaviour for n=0 is undefined */
        return 0;

    idx_t count = 0;
    seq_t k = 0;

    for (;; ++k) {
        seq_t val = 5;
        val *= k;        /* val = 5*k                        */
        val *= k;        /* val = 5*k^2                      */
        val *= k;        /* val = 5*k^3                      */
        val += 3;        /* val = 5*k^3 + 3                  */

        if (is_prime_seq(val)) {
            ++count;
            if (count == n)
                return val;
        }
    }
}
