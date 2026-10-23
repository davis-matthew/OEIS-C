/*
 * A141721  A141631(n) mod 10.
 * https://oeis.org/A141721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = a(n-10), period 10 (2, 7, 8, 5, 8, 7, 2, 3, 0, 3)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic pattern with a(1)=2 */
    static const seq_t pattern[10] = {2, 7, 8, 5, 8, 7, 2, 3, 0, 3};
    /* Adjust for 1‑based indexing: element 0 corresponds to a(1) */
    size_t idx = (size_t)((n - 1) % 10);
    return pattern[idx];
}
