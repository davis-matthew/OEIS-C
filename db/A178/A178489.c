/*
 * A178489  a(n) = floor(n^(1/6)): integer part of sixth root of n.
 * https://oeis.org/A178489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n^(1/6))
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   power_of_sixth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uintptr_t, if needed */
#include "../utils/power_of_sixth.h"

seq_t a(idx_t n)
{
    /* The limit value we're comparing against, in seq_t. */
    seq_t limit = (seq_t)n;
    seq_t k = 0;
    while (1) {
        seq_t cand = power_of_sixth(k + 1);
        if (cand <= limit)
            ++k;
        else
            break;
    }
    return k;
}
