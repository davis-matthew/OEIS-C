/*
 * A011535  Numbers that contain a 5.
 * https://oeis.org/A011535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose decimal representation
 *            contains a digit 5
 * Range:     u8 n <= 49, u16 n <= 3439, u32 n <= 3439, u64 n <= 3439,
 *            u128 n <= 3439, big any n
 * Helpers:   has_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_five.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* Undefined for n < 1; return 0 as a safe sentinel */

    seq_t count = 0;
    seq_t x = 0;

    while (count < n) {
        x++;
        if (has_five(x))
            count++;
    }
    return x;
}
