/*
 * A031466  Numbers whose base-4 representation has one fewer 0 than 3's.
 * https://oeis.org/A031466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer whose base‑4 representation contains one
 *            more 3 than 0. Equivalently, count_4(n,3) = count_4(n,0)+1
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_one_more_three_than_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/has_one_more_three_than_zero.h"

/* Return the n‑th term (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0 */

    seq_t cur   = 1;
    idx_t found = 0;

    while (1) {
        if (has_one_more_three_than_zero(cur)) {
            ++found;
            if (found == n) return cur;
        }
        ++cur;
    }
}
