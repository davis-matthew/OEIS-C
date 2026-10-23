/*
 * A039263  Numbers whose base-12 representation has the same number of 6's
 *          and 10's.
 * https://oeis.org/A039263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th integer x whose base‑12 representation * has the
 *            same number of digit 6 as digit 10
 * Range:     u8 n <= 187, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   equal_6_and_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/equal_6_and_10.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n < 1, return 0 as a safe value */
        return (seq_t)0;

    seq_t cur = (seq_t)0;
    idx_t  cnt = 0;

    while (1) {
        if (equal_6_and_10(cur)) {
            ++cnt;
            if (cnt == n)
                return cur;
        }
        ++cur;          /* next integer to test */
    }
}
