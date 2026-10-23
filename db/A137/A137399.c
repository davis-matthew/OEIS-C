/*
 * A137399  a(0) = 1, a(1) = a(2) = 2, a(3) = 1, and a(n) = 4*a(n-4) for n >
 *          3.
 * https://oeis.org/A137399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(floor(n/4)) * init[n mod 4]
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pow4, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                     init = {1, 2, 2, 1}                     */

#include "../utils/pow4.h"   /* seq_t pow4(idx_t exp);          */
#include "../utils/seq_mul.h"/* seq_t seq_mul(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* Base values for the first cycle. */
    static const seq_t init[4] =
    {
        (seq_t)1,
        (seq_t)2,
        (seq_t)2,
        (seq_t)1
    };

    if (n < 4)
        return init[n];

    /* Compute 4^(n/4) as 4^k where k = n>>2. */
    idx_t k = n >> 2;              /* floor(n/4) */
    seq_t mult = pow4(k);          /* 4^k */

    /* Result is 4^k * init[n mod 4]. */
    return seq_mul(mult, init[n & 3]);
}
