/*
 * A010378  Squares mod 15.
 * https://oeis.org/A010378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 15 * floor((n-1)/6) + [0,1,4,6,9,10][(n-1)%6]
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t residues[6] = {0, 1, 4, 6, 9, 10};
    idx_t k = n - 1;
    idx_t q = k / 6;
    idx_t r = k % 6;
    idx_t block = q * 15;
    return (seq_t)(block + residues[(size_t)r]);
}
