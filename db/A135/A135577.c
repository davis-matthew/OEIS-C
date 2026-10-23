/*
 * A135577  Numbers that have only the digit "1" as first, central and final
 *          digit. For numbers with 5 or more digits the rest of digits are
 *          "0".
 * https://oeis.org/A135577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = 1 + 10^(n-1) + 100^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset, but safe */
    if (n == 1)
        return (seq_t)1;
    seq_t p10  = pow10(n - 1);            /* 10^(n-1) */
    seq_t p100 = pow10((n - 1) * 2);      /* 10^(2*(n-1)) = 100^(n-1) */
    return (seq_t)1 + p10 + p100;
}
