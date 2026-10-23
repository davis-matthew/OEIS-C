/*
 * A041056  Numerators of continued fraction convergents to sqrt(34).
 * https://oeis.org/A041056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 70*a(n-4) - a(n-8)  for n ≥ 8.
   Initial terms: a(0)=5, a(1)=6, a(2)=29, a(3)=35,
                  a(4)=379, a(5)=414, a(6)=2035, a(7)=2449. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)5, (seq_t)6, (seq_t)29, (seq_t)35,
        (seq_t)379, (seq_t)414, (seq_t)2035, (seq_t)2449
    };

    if (n < 8)
        return init[(size_t)n];

    /* Maintain a circular buffer of the last 8 computed terms. */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (size_t i = 8; i <= n; ++i) {
        /* Compute: a(i) = 70*a(i-4) - a(i-8) */
        seq_t prod = mul_small(buf[(i - 4) % 8], (unsigned int)70);
        buf[i % 8] = prod - buf[(i - 8) % 8];
    }

    return buf[n % 8];
}
