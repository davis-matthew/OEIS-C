/*
 * A071193  Least m > n such that bigomega(m) != bigomega(n), where
 *          bigomega(n) = A001222(n).
 * https://oeis.org/A071193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least m > n such that bigomega(m) != bigomega(n) */
/* Computes the difference of the total number of prime factors
   (counted with multiplicity) of successive integers until a change. */

#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    seq_t omega_n = bigomega(n);
    idx_t m = n + 1;
    while (bigomega(m) == omega_n) {
        ++m;
    }
    return (seq_t)m;
}
