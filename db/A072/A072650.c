/*
 * A072650  Starting from the right (the least significant end) rewrite 0 to 0
 *          and x1 to 1 in the binary expansion of n.
 * https://oeis.org/A072650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = value obtained by scanning the binary expansion of n *
 *            from the least‐significant bit: output a 0 for each 0, and
 *            output a 1 * for each 1 while skipping the bit to its left. This
 *            reproduces OEIS * A072650
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)0;

    /* find index of most significant set bit in n  */
    idx_t max_bit = 0;
    idx_t tmp = n;
    while (tmp >>= 1)
        ++max_bit;

    uint64_t r = 0;           /* intermediate accumulator (fits all cases) */
    idx_t bit_pos = 0;        /* position in result */
    idx_t i = 0;              /* current index in original number */

    while (i <= max_bit) {
        if ((n >> i) & 1ULL) {          /* bit is 1: output 1, skip next bit */
            r |= (uint64_t)1 << bit_pos;
            ++bit_pos;
            i += 2;                     /* skip the bit to the left of the 1 */
        } else {                         /* bit is 0: output 0 (nothing to do) */
            ++bit_pos;
            ++i;
        }
    }

    return (seq_t)r;
}
