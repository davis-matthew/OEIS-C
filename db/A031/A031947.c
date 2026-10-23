/*
 * A031947  Numbers in which 0,1,2,3,4,5 all occur in base 6.
 * https://oeis.org/A031947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_all_digits_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A031947
 *  Formula: a(n) = the nth positive integer whose base‑6 representation
 *  contains all digits 0,1,2,3,4,5 at least once. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_all_digits_base6.h"

/*  Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* beyond offset – undefined */
    idx_t  count = 0;
    seq_t  value = 8345 - 1;              /* start just before first term */

    while (count < n) {
        ++value;
        if (has_all_digits_base6(value))
            ++count;
    }
    return value;
}
