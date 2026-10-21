/*
 * A065502  Positive numbers divisible by 2 or 5; 1/n not purely periodic
 *          after decimal point.
 * https://oeis.org/A065502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 153, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A065502
   Positive numbers divisible by 2 or 5.
   Formula: a(n) = 10 * floor((n-1)/6) + p[(n-1) mod 6]
   where p = {2,4,5,6,8,10}   (indices 0..5)
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t q = (n - 1) / 6;       /* number of full 10‑cycle blocks before n */
    idx_t r = (n - 1) % 6;       /* index inside the current block (0..5) */
    seq_t base = (seq_t)q * 10;  /* start of the block */
    static const seq_t pat[6] = { 2, 4, 5, 6, 8, 10 };
    return base + pat[r];
}
