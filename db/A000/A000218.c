/*
 * A000218  Take sum of squares of digits of previous term; start with 3.
 * https://oeis.org/A000218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A003132(a(n-1)) = sum of squares of the digits of a(n-1),
 *            a(1) = 3. Eventually periodic with period 8: for n >= 6, a(n) =
 *            a(n-8)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   a218_step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a218_step.h"

seq_t a(idx_t n)
{
    unsigned x = 3;
    idx_t i, steps;
    if (n < 1)
        return (seq_t)0;
    steps = n - 1;
    /* Once the cycle is reached (from index 6), the period is 8 */
    if (steps >= 13)
        steps = 5 + (steps - 5) % 8;
    for (i = 0; i < steps; i++)
        x = a218_step(x);
    return (seq_t)x;
}
