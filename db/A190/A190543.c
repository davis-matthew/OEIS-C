/*
 * A190543  a(n) = 8^n - 3^n.
 * https://oeis.org/A190543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 3^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Use exponentiation by squaring from pow_seq.
       pow_seq is defined in the utils package and works for
       any seq_t type (unsigned integers or arbitrary‑precision.
    */
    seq_t pow8 = pow_seq((seq_t)8, (int)n);
    seq_t pow3 = pow_seq((seq_t)3, (int)n);
    return pow8 - pow3;
}
