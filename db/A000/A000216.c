/*
 * A000216  Take sum of squares of digits of previous term, starting with 2.
 * https://oeis.org/A000216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 2, a(n) = A003132(a(n-1)) (sum of squares of digits of
 *            a(n-1)); periodic with period 8 for n >= 2, so we iterate 1 +
 *            ((n-2) mod 8) steps from 2
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   a000216_step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000216_step.h"

seq_t a(idx_t n)
{
    unsigned v = 2;
    idx_t steps, i;
    if (n <= 1)
        return (seq_t)2;
    steps = 1 + (n - 2) % 8;
    for (i = 0; i < steps; i++)
        v = a000216_step(v);
    return (seq_t)v;
}
