/*
 * A191256  Duplicate of A067368.
 * https://oeis.org/A191256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n-th even integer that is congruent to 2 (mod 4) or
 *            16 (mod 32)
 * Range:     i8 n <= 17, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) {
        abort();               /* sequence is 1‑based */
    }
    idx_t count = 0;
    seq_t x = 2;   /* first even integer */

    for (;; x += 2) {                 /* iterate over all even integers */
        if ((x % 4) == 2 || (x % 32) == 16) {   /* test the criterion */
            if (++count == n) {
                return x;
            }
        }
    }
}
