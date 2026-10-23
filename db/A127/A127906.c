/*
 * A127906  a(n) = (n in base 10) * (n in base 2).
 * https://oeis.org/A127906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * (decimal value of binary representation of n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

/* Convert an index n to its decimal value when the bit pattern
 * of n is interpreted as a decimal number.
 */
static seq_t binary_to_decimal(idx_t n)
{
    /* n == 0 is a special case: binary representation is "0" */
    if (n == 0)
        return 0;

    /* Find the position of the most significant bit of n. */
    idx_t msb = floor_log2_uint64(n);

    seq_t dec = 0;
    for (idx_t i = msb; ; --i) {
        seq_t bit = (seq_t)((n >> i) & 1ULL);
        dec = dec * 10 + bit;
        if (i == 0)
            break;
    }
    return dec;
}

/* Sequence a(n) = (decimal value of n) * (decimal value of binary representation of n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t dec = binary_to_decimal(n);
    return (seq_t)n * dec;
}
