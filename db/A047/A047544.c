/*
 * A047544  Numbers that are congruent to {1, 3, 4, 7} mod 8.
 * https://oeis.org/A047544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + offset, where offset = {1, 3, 4, 7}
 *            according to (n-1) mod 4
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for index 0 */
    idx_t q = (n - 1) / 4;              /* quotient of full blocks of 8 */
    idx_t r = (n - 1) % 4;              /* position inside the block (0‑3) */
    seq_t base = ((seq_t)8) * ((seq_t)q);/* 8 * q, cast to seq_t */
    switch (r) {
        case 0: return base + (seq_t)1;
        case 1: return base + (seq_t)3;
        case 2: return base + (seq_t)4;
        default: return base + (seq_t)7;     /* r == 3 */
    }
}
