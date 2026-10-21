/*
 * A090089  Smallest even pseudoprimes to odd base=4n-1, not necessarily
 *          exceeding n.
 * https://oeis.org/A090089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest even composite x such that (4n-1)^(x-1) ≡ 1
 *            (mod x). The implementation searches even numbers from 4
 *            upwards, testing each with mod_pow
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* odd base = 4n-1 */
    seq_t base = (seq_t)((seq_t)4 * (seq_t)n - (seq_t)1);

    /* search for the smallest even composite x ≥ 4 */
    for (idx_t x = 4; ; x += 2)
    {
        seq_t xm = (seq_t)x;                     /* modulus in seq_t */
        uint64_t exp = (uint64_t)(x - (idx_t)1); /* exponent fits in 64 bits */

        if (mod_pow(base, exp, xm) == (seq_t)1)
            return xm;                           /* found the pseudoprime */
    }
}
