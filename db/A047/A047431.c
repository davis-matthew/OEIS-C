/*
 * A047431  Numbers that are congruent to {1, 4, 5, 6} mod 8.
 * https://oeis.org/A047431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + [1,4,5,6][(n-1)%4]
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence lists all positive integers congruent to 1,4,5,6 (mod 8). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑indexed.  Compute the k‑th block and the offset within that block. */
    seq_t k = (seq_t)(n - 1) / 4;          /* block index (0‑based) */
    seq_t m = 8 * k;                      /* base value of the block            */
    idx_t r = (idx_t)((n - 1) % 4);        /* offset within the block            */

    switch (r) {
        case 0: return m + 1;             /* 8k+1 */
        case 1: return m + 4;             /* 8k+4 */
        case 2: return m + 5;             /* 8k+5 */
        default: return m + 6;            /* r==3 => 8k+6 */
    }
}
