/*
 * A000149  a(n) = floor(e^n).
 * https://oeis.org/A000149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(e^n), with e^n = 1 + n/1*(1 + n/2*(1 + n/3*(1 + ...
 *            (1 + n/K)))), evaluated by Horner's rule in multi-limb fixed
 *            point and truncated to its integer part
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   a149_try
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a149_try.h"

seq_t a(idx_t n)
{
    unsigned G = 2;
    seq_t r = 0;
    while (!a149_try(n, G, &r))
        G += 4;
    return r;
}
