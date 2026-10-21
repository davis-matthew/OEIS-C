/*
 * A074994  Floor of concatenation of n, n+1, n+2, n+3 divided by 4.
 * https://oeis.org/A074994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor( concat(n, n+1, n+2, n+3) / 4 )
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Concatenate n, n+1, n+2, n+3 into a single decimal number. */
    seq_t val = (seq_t)n;

    val = val * pow10(digits(n + 1)) + (seq_t)(n + 1);
    val = val * pow10(digits(n + 2)) + (seq_t)(n + 2);
    val = val * pow10(digits(n + 3)) + (seq_t)(n + 3);

    return val / 4;
}
