/*
 * A190785  Numbers that are congruent to {0, 2, 3, 5, 7, 9, 11} mod 12.
 * https://oeis.org/A190785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = base[(n-1) mod 7] + 12 * floor((n-1)/7)
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[7] = {0, 2, 3, 5, 7, 9, 11};

    if (n == 0u)
        return (seq_t)0;   /* undefined for n < 1, but return safe value */

    idx_t q = (n - 1u) / 7u;   /* block number */
    idx_t r = (n - 1u) % 7u;   /* position inside the block */

    return base[(size_t)r] + (seq_t)q * (seq_t)12;
}
