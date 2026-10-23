/*
 * A032887  Numbers whose base-8 representation Sum_{i=0..m} d(i)*8^i has d(0)
 *          < d(1) > d(2) < ...
 * https://oeis.org/A032887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) generates the n-th natural number whose base‑8 digits
 *            satisfy the alternating pattern d0 < d1 > d2 < d3 > … (indexing
 *            from the least significant digit). The function scans successive
 *            integers in ascending order and counts those that meet the
 *            pattern until the n‑th one is found. The implement...
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_valid_oct_alternating
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_valid_oct_alternating.h"

seq_t a(idx_t n)
{
    idx_t seen = 0;          /* number of valid terms encountered */
    seq_t val = 0;           /* candidate number                 */

    while (seen < n) {
        val++;               /* try next integer                  */
        if (is_valid_oct_alternating(val))
            seen++;
    }
    return val;
}
