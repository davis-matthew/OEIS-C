/*
 * A163584  Number of singularities of tan(x) in integer intervals starting
 *          with (0,1).
 * https://oeis.org/A163584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count_sings(n+1) - count_sings(n) where count_sings(L) =
 *            0 if L*226 <= 355, else count_sings(L) = floor(((L*226 - 355) *
 *            113) / 80230) + 1. The constants come from the rational
 *            approximation 355/113 of π
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   count_sings
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/count_sings.h"

/* required function */
seq_t a(idx_t n)
{
    seq_t c1 = count_sings(n);
    seq_t c2 = count_sings(n + 1);
    return c2 - c1;
}
