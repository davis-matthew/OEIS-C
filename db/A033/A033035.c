/*
 * A033035  Numbers such that all base 9 digits are odd.
 * https://oeis.org/A033035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(4n+j) = 9*a(n)+2*j-1 (j=1..4) base cases: a(1)=1, a(2)=3,
 *            a(3)=5, a(4)=7
 * Range:     u8 n <= 38, u16 n <= 1364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the four base cases directly. */
    if (n <= 4)
        return 2 * (seq_t)n - 1;

    /* Stack to hold the “add” terms for each recursion level.
       The depth is at most log4(n), at most 64 for 64‑bit indices. */
    unsigned add[64];
    size_t depth = 0;

    while (n > 4) {
        unsigned r = (unsigned)((n - 1) % 4);   /* 0..3 */
        add[depth++] = 2 * r + 1;               /* (2*j-1) with j=r+1 */
        n = (n - 1) / 4;
    }

    /* n is now a base case index (1..4). */
    seq_t val = 2 * (seq_t)n - 1;

    /* Build the result by applying the stored terms from the last
       level (closest to the leaf) up to the root. */
    while (depth--) {
        val = val * 9 + add[depth];
    }

    return val;
}
