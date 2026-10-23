/*
 * A039251  Numbers whose base-12 representation has the same number of 4's
 *          and 9's.
 * https://oeis.org/A039251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   equal_4_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number in OEIS A039251 (offset 1):
   Numbers whose base‑12 representation has the same number of 4's and 9's.
   The function searches sequentially from 0, counting numbers that satisfy
   the property until it reaches the specified index n. */
#include <stdbool.h>
#include "../utils/equal_4_9.h"

seq_t a(idx_t n)
{
    if (n == 0)             /* behavior below offset is unspecified; return 0 */
        return 0;
    idx_t target = n;
    idx_t count = 0;
    seq_t x = 0;

    while (1) {
        if (equal_4_9(x)) {
            ++count;
            if (count == target)
                return x;
        }
        ++x;
    }
}
