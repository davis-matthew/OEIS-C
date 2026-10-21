/*
 * A079314  Number of first-quadrant cells (including the two boundaries) born
 *          at stage n of the Holladay-Ulam cellular automaton.
 * https://oeis.org/A079314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=3^(popcount(n)-1)+1 for n>0
 * Range:     u8 n <= 126, u16 n <= 4094, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    int pc = __builtin_popcountll((unsigned long long)n);
    seq_t exp = (seq_t)(pc - 1);

    /* big_pow returns 3^exp as a seq_t, then add 1 */
    return big_pow((seq_t)3, exp) + (seq_t)1;
}
