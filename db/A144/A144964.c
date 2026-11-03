/*
 * A144964  Number of groves of degree n.
 * https://oeis.org/A144964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^Catalan(n) - 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 8 terms through n = 8 (2026-11-01)
 * Helpers:   catalan, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t cat = catalan(n);
    seq_t res = ipow((seq_t)2, cat);
    return res - 1;
}
