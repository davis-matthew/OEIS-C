/*
 * A176805  a(n) = 3^n + 3*n + 1.
 * https://oeis.org/A176805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 3*n + 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* compute 3^n */
    seq_t power = pow3(n);

    /* add 3*n + 1 */
    power += (seq_t)3 * n + 1;

    return power;
}
