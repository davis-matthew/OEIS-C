/*
 * A126644  a(n) = 3*3^n - 3*2^n + 1.
 * https://oeis.org/A126644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*3^n - 3*2^n + 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Powered by the utilities pow2 and pow3 which compute 2^n and 3^n in seq_t */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute 3^n and 2^n */
    seq_t p3 = pow3(n);
    seq_t p2 = pow2(n);

    /* a(n) = 3*3^n - 3*2^n + 1 */
    return 3 * p3 - 3 * p2 + 1;
}
