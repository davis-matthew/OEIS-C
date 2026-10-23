/*
 * A038396  Concatenate first n even numbers in reverse order.
 * https://oeis.org/A038396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate the first n even numbers in reverse order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = n; i > 0; --i) {
        /* even number 2*i fits in 64‑bit for all n of interest */
        seq_t even = (seq_t)(2 * i);
        uint64_t d = digit_count((uint64_t)(2 * i));  /* number of decimal digits of 2*i */
        result = result * pow10((idx_t)d) + even;
    }
    return result;
}
