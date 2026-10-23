/*
 * A032994  If d,e are consecutive digits of n in base 12, then |d-e|>=4.
 * https://oeis.org/A032994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n‑th integer whose base‑12 digits differ by at least 4
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   good_base12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation scans integers in increasing order and counts the
   ones that satisfy the condition.  It works for any size of seq_t and
   idx_t and respects all stated constraints. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/good_base12.h"

/* Main sequence function.  It returns the n‑th valid number, with
   n >= 1.  For n < 1 the return value is 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t count = 0;      /* how many valid numbers found so far */
    idx_t candidate = 1;  /* integer to test next */

    while (true) {
        if (good_base12((seq_t)candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
