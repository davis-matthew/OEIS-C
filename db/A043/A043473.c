/*
 * A043473  Numbers having exactly one 5 in base 9.
 * https://oeis.org/A043473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   has_one_five_in_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having exactly one 5 in base 9.
   Search implementation. Look for the n-th integer whose
   base‑9 representation contains a single digit 5. */

#include <stdbool.h>
#include "../utils/has_one_five_in_base9.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;      /* for safety, though n>=1 per OEIS */
    seq_t val = (seq_t)5;
    idx_t reached = (idx_t)1;
    while (reached < n) {
        ++val;
        if (has_one_five_in_base9(val))
            ++reached;
    }
    return val;
}
