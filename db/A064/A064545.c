/*
 * A064545  Lesser of two consecutive primes such that n*p + q is a perfect
 *          square, p < q.
 * https://oeis.org/A064545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A064545: the smaller of consecutive primes p < q for which
   n*p + q is a perfect square.  For a given n the function scans
   primes until such a pair is found and returns p. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe guard */

    idx_t p = 2;                      /* first prime */
    while (true) {
        idx_t q = next_prime(p);     /* next prime after p   */
        seq_t val = (seq_t)n * (seq_t)p + (seq_t)q;
        seq_t r   = floor_sqrt(val);
        if (r * r == val)            /* found a square   */
            return (seq_t)p;         /* smaller prime   */
        p = q;                       /* advance to next pair */
    }
}
