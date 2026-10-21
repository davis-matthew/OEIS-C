/*
 * A232803  Odd primes, twice odd primes, 4, and 8.
 * https://oeis.org/A232803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: all odd primes, twice odd primes, 4, and 8 */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Return the n-th term of the sequence.
       n is 1‑based. Assumes n >= 1. */
    idx_t count = 0;
    idx_t m = 3;                     /* first term is 3 */
    for (;; ++m) {
        bool ok = false;
        if (m == 4 || m == 8) {              /* the special numbers */
            ok = true;
        } else if ((m & 1) && is_prime(m)) { /* odd prime */
            ok = true;
        } else if ((m & 1) == 0) {           /* even number */
            idx_t half = m >> 1;             /* m / 2 */
            if (half >= 3 && is_prime(half)) /* twice odd prime */
                ok = true;
        }
        if (ok) {
            ++count;
            if (count == n) {
                return (seq_t)m;
            }
        }
    }
}
