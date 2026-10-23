/*
 * A039387  Numbers whose base-9 representation has the same nonzero number of
 *          5's and 6's.
 * https://oeis.org/A039387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_nonzero_5_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A039387: numbers whose base-9 representation has the same positive
   number of 5's and 6's – obtained by scanning integers and keeping those
   that satisfy the digit property. */

#include <stdbool.h>
#include "../utils/has_equal_nonzero_5_6.h"

/* Return the n-th term of sequence A039387 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; behaviour for 0 is arbitrary */

    idx_t count = 0;
    seq_t candidate = 1;

    for (;;) {
        if (has_equal_nonzero_5_6(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
