/*
 * A164945  Decimal value of the concatenation of first n multiples of 3 in
 *          binary.
 * https://oeis.org/A164945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal value of the concatenation of the first n
 *            multiples of 3 when the bit string is interpreted as a binary
 *            number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation iteratively builds the concatenated
   binary string by left‑shifting the current result by the bit
   length of the next multiple and then adding the multiple itself.
   It works for any seq_t that supports the operators +, <<, and
   for which the shift count is a non‑negative integer. */
#include <stdint.h>

static unsigned int bitlen(idx_t x)
{
    unsigned int len = 0;
    do
    {
        ++len;
        x >>= 1;
    } while (x);
    return len;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t m   = 3 * k;
        unsigned int len = bitlen(m);
        result = (result << len) + (seq_t)m;
    }
    return result;
}
