/*
 * A141266  Concatenate first n double factorials.
 * https://oeis.org/A141266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1)*10^⌊1+log10(n!!)⌋ + n!! (concatenation of decimal
 *            representations), a(1) = 1. The recurrence is implemented by
 *            repeatedly multiplying the current result by 10 to the power of
 *            the digit length of the new double factorial and then adding
 *            that double factorial
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    seq_t value = 1;            /* current concatenated result */
    seq_t odd_df = 1;           /* double factorials for odd indices */
    seq_t even_df = 1;          /* double factorials for even indices */

    for (idx_t k = 2; k <= n; ++k) {
        if ((k & 1) == 0) {      /* even k */
            even_df = even_df * (seq_t)k;
            idx_t d = digit_len(even_df);
            value = value * pow10((idx_t)d) + even_df;
        } else {                 /* odd k */
            odd_df = odd_df * (seq_t)k;
            idx_t d = digit_len(odd_df);
            value = value * pow10((idx_t)d) + odd_df;
        }
    }
    return value;
}
