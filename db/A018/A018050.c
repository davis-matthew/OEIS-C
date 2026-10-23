/*
 * A018050  Powers of fourth root of 2 rounded up.
 * https://oeis.org/A018050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(2^(n/4))
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only.  It exploits that the
 * fractional part of 2^(n/4) can be rounded up by checking the
 * remainder of n modulo 4.  The integer fourth root of 2^n is obtained
 * as the floor of the square root taken twice:  floor_sqrt(floor_sqrt(2^n)).
 * The sequence values are computed as:
 *   if (n % 4 == 0) return floor_sqrt(floor_sqrt(2^n));
 *   else             return floor_sqrt(floor_sqrt(2^n)) + 1; */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t definition of idx_t          */
#include "../utils/pow2.h"        /* pow2(idx_t exp) -> seq_t */
#include "../utils/floor_sqrt.h"  /* floor_sqrt(seq_t) -> seq_t */

seq_t a(idx_t n)
{
    /* 2^n as a seq_t */
    seq_t two_n = pow2(n);

    /* floor of sqrt(2^n)   */
    seq_t r = floor_sqrt(two_n);

    /* floor of sqrt(floor_sqrt(2^n)) == floor fourth root of 2^n */
    r = floor_sqrt(r);

    /* if n is not a multiple of 4, round up */
    if (n & 0x3)  /* n % 4 != 0 */
        ++r;

    return r;
}
