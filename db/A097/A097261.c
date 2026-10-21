/*
 * A097261  Numbers whose set of base 15 digits is {0,E}, where E base 15 = 14
 *          base 10.
 * https://oeis.org/A097261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 14*A033051(n) Recurrence: a(0) = 0; a(2n) = 15*a(n)
 *            a(2n+1) = a(2n) + 14 The recurrence can be implemented by
 *            scanning the binary representation of n from the most
 *            significant bit down to the least and updating the result as a =
 *            15*a + 14*(bit)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t msb_idx(idx_t n)
{
    /* position of the most significant set bit, 0‑based;
       safe to call only with n > 0 */
    idx_t pos = 0;
    while (n >> pos) ++pos;
    return pos - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t res = (seq_t)0;
    idx_t msb = msb_idx(n);

    for (idx_t i = msb; ; --i) {
        seq_t bit = (seq_t)((n >> i) & (idx_t)1);
        res = res * (seq_t)15 + (seq_t)(14 * bit);
        if (i == 0) break;
    }
    return res;
}
