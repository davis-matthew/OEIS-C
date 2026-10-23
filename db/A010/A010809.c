/*
 * A010809  21st powers: a(n) = n^21.
 * https://oeis.org/A010809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^21
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 8, u128 n <= 68,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    return pow_seq((seq_t)n, 21);
}
