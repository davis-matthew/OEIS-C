/*
 * A047363  Numbers that are congruent to {0, 2, 3, 4, 5} mod 7.
 * https://oeis.org/A047363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*floor((n-1)/5) + offset[(n-1)%5], offset = {0,2,3,4,5}
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A047363: numbers ≡ {0,2,3,4,5} mod 7, offset 1. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute integer division and remainder with respect to 5 */
    idx_t k = (n - 1) / 5;          /* floor((n-1)/5) */
    idx_t r = (n - 1) % 5;          /* (n-1) mod 5 */

    /* offsets for the five allowed residues mod 7 */
    static const seq_t offs[5] = { 0, 2, 3, 4, 5 };

    /* compute 7*k in idx_t to avoid overflow of seq_t if k is large,
       then cast to seq_t for the final result */
    seq_t base = (seq_t)(7 * k);
    return base + offs[r];
}
