/*
 * A219251  Numbers such that the absolute difference of a pair of adjacent
 *          decimal digits is not prime.
 * https://oeis.org/A219251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   valid_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers such that NOT all adjacent decimal digit differences are prime. */
/* In other words, a number is in the sequence if it has at least one pair of adjacent
   decimal digits whose absolute difference is NOT 2, 3, 5, or 7. */

#include <stdbool.h>
#include "../utils/valid_num.h"

/* Return the n-th element of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined by OEIS convention */

    idx_t  count   = 0;
    seq_t  cand    = 10;                 /* smallest member of the sequence */

    for (;;) {
        if (valid_num(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
        ++cand;
    }
}
