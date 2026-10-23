/*
 * A017099  a(n) = (8*n + 2)^11.
 * https://oeis.org/A017099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 2)^11
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 6, u128 n <= 397,
 *            big any n
 * Helpers:   pow_seq_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_11.h"

seq_t a(idx_t n)
{
    /* compute (8*n + 2) as seq_t and raise it to the 11th power */
    seq_t base = (seq_t)8 * n + 2;
    return pow_seq_11(base);
}
