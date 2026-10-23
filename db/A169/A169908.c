/*
 * A169908  a(n) = n*n in carryless digital root arithmetic in base 10.
 * https://oeis.org/A169908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*n in carryless digital‑root arithmetic (base 10)
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   carryless_square, dr_prod, dr_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/carryless_square.h"
#include "../utils/dr_prod.h"
#include "../utils/dr_sum.h"

seq_t a(idx_t n)
{
    return carryless_square((seq_t)n);
}
