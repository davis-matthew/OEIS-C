/*
 * A231472  Largest integer less than 12 and coprime to n.
 * https://oeis.org/A231472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 822, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for k from 11 down to 1 return the first k with
   gcd(k, n) == 1.  This yields the largest integer < 12
   that is coprime to n. */
#include "../utils/gcd.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    for (uint8_t k = 11; k >= 1; --k) {
        if (gcd(sn, (seq_t)k) == 1U) {
            return (seq_t)k;
        }
    }
    /* unreachable: all numbers <= 11 share a common factor with n,
       but that only happens for n == 0, which is not a valid input.
       Return 0 to silence compilers that expect a return on all paths. */
    return 0;
}
