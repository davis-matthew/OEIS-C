/*
 * A045508  Concatenate factorials.
 * https://oeis.org/A045508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of k! for 1 ≤ k ≤ n in decimal
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t factorial = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        factorial = factorial * (seq_t)i;
        int d = digit_len(factorial);            /* number of decimal digits of i! */
        result = result * pow10((idx_t)d) + factorial;
    }
    return result;
}
