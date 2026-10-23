/*
 * A120433  Numbers whose Roman numeral representation uses the subtractive
 *          notation.
 * https://oeis.org/A120433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 1952, u32 n <= 1952, u64 n <= 1952,
 *            u128 n <= 1952, big any n
 * Helpers:   contains_49
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Linear search for the n-th positive integer whose decimal
   representation contains a digit 4 or 9.  This characterizes the
   numbers whose Roman‑numeral representation uses subtractive notation. */

#include <stdbool.h>
#include "../utils/contains_49.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset starts at 1 */

    idx_t  found = 0;
    seq_t  num   = 1;

    for (;;) {
        if (contains_49(num)) {
            ++found;
            if (found == n) return num;
        }
        num += 1;
    }
}
