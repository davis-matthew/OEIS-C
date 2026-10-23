/*
 * A008854  Numbers that are congruent to {0, 1, 4} mod 5.
 * https://oeis.org/A008854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*floor((n-1)/3) + {0,1,4} for (n-1 mod 3) = 0,1,2
 *            respectively
 * Range:     u8 n <= 154, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based; the sequence is defined for n >= 1 */
    idx_t n1 = n - 1;                /* shift to 0-based */
    seq_t base = 5 * ((seq_t)n1 / 3);/* common part 5*floor((n-1)/3) */

    switch (n1 % 3) {
        case 0: return base;        /* {0, 5, 10, ...}          */
        case 1: return base + 1;    /* {1, 6, 11, ...}          */
        case 2: return base + 4;    /* {4, 9, 14, ...}          */
    }
    /* unreachable */
    return base;
}
