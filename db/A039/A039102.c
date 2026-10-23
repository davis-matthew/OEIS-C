/*
 * A039102  Numbers whose base-9 representation has the same number of 4's and
 *          5's.
 * https://oeis.org/A039102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal45
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the nth integer (1‑indexed) whose base‑9 representation contains an equal number of 4’s and 5’s */
#include <stdbool.h>
#include "../utils/has_equal45.h"

/* Return the nth (n ≥ 1) term of OEIS A039102. */
seq_t a(idx_t n) {
    idx_t count = 0;   /* how many valid numbers have been found so far */
    seq_t num = 0;     /* current integer being tested */

    while (1) {
        if (has_equal45(num)) {
            count++;
            if (count == n) return num;
        }
        num++;          /* move to the next integer */
    }
}
