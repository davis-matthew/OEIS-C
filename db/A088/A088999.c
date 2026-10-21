/*
 * A088999  Flip 6,9 numbers: if number k contains decimal digit '6' or '9',
 *          then flip all occurrences of '6' to '9' and vice versa.
 * https://oeis.org/A088999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   contains_69, flip_69
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A088999: Flip 6,9 numbers.
   a(n) is the n-th (starting from 1) integer that contains at least one
   decimal digit 6 or 9, after swapping all 6's and 9's in that integer. */

#include <stdbool.h>
#include "../utils/contains_69.h"
#include "../utils/flip_69.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num   = 1;
    while (1) {
        if (contains_69(num)) {
            ++count;
            if (count == n) return flip_69(num);
        }
        ++num;
    }
}
