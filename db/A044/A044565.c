/*
 * A044565  Numbers n such that string 6,4 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer m such that the base‑7 representation of
 *            m contains the substring “64” but the base‑7 representation of
 *            m+1 does not. This implementation searches integers in
 *            increasing order until the n‑th such number is found
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_substring_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_substring_64.h"

seq_t a(idx_t n)
{
    idx_t found = 0;              /* how many qualifying numbers we have seen */
    seq_t m = 1;                  /* current integer to test */

    while (true) {
        if (has_substring_64(m) && !has_substring_64(m + 1)) {
            ++found;
            if (found == n)
                return m;
        }
        ++m;
    }
}
