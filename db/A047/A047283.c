/*
 * A047283  Numbers that are congruent to {0, 1, 3, 6} mod 7.
 * https://oeis.org/A047283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/4) + {0,1,3,6}[(n-1) mod 4]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, a(3) = 3, a(4) = 6, a(5) = 7, ... */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n < 1 as 0 (offset is 1, so this case is irrelevant) */
    if (n < 1)
        return (seq_t)0;

    /* Zero‑based index of the n-th included number */
    idx_t zero = n - 1;

    /* How many complete blocks of 4 included numbers we skip */
    idx_t block = zero / 4;
    /* Position within the current block */
    idx_t r = zero % 4;

    seq_t base = (seq_t)block * (seq_t)7;

    switch (r) {
    case 0: return base;          /* 0 modulo 7 */
    case 1: return base + (seq_t)1;  /* 1 modulo 7 */
    case 2: return base + (seq_t)3;  /* 3 modulo 7 */
    case 3: return base + (seq_t)6;  /* 6 modulo 7 */
    default: return base;          /* unreachable */
    }
}
