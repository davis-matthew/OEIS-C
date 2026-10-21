/*
 * A056021  Numbers k such that k^4 == 1 (mod 5^2).
 * https://oeis.org/A056021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)/4)*25 + {1,7,18,24}[(n-1)%4]
 * Range:     u8 n <= 41, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0, return 0 */

    const seq_t residues[4] = { (seq_t)1, (seq_t)7, (seq_t)18, (seq_t)24 };
    idx_t q = (n - 1) / 4;            /* number of complete blocks of 4 */
    idx_t r = (n - 1) % 4;            /* position within the block */

    /* Compute using a larger unsigned integer to avoid overflow on the
       intermediate multiplication, then cast back to seq_t. */
    uint64_t val = (uint64_t)q * 25ULL + (uint64_t)residues[r];
    return (seq_t)val;
}
