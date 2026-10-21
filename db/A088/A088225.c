/*
 * A088225  Solutions to x^n == 7 (mod 11).
 * https://oeis.org/A088225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)/4 * 11 + (n-1)%4 mapped to residues {2,6,7,8} mod
 *            11
 * Range:     u8 n <= 93, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset indices starting from 0 */
    idx_t off = n - 1;
    idx_t q = off / 4;           /* how many full blocks of 11 */
    idx_t r = off % 4;           /* position within the block (0..3) */

    /* good residues in order */
    static const seq_t good[4] = {2, 6, 7, 8};

    return (seq_t)11 * (seq_t)q + good[r];
}
