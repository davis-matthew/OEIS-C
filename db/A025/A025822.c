/*
 * A025822  Expansion of 1/((1-x^2)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A025822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{c=0..⌊n/11⌋} ( [ (n-11c) even ] * (⌊(n-11c)/8⌋ + 1 ) )
 *            (counts partitions of n into parts 2, 8, and 11)
 * Range:     u8 n <= 285, u16 n <= 4787, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* loop over number of 11-parts */
    idx_t maxc = n / 11ULL;
    for (idx_t c = 0ULL; c <= maxc; ++c)
    {
        idx_t rem = n - 11ULL * c;
        if ((rem & 1ULL) != 0ULL)   /* skip odd remainders */
            continue;
        /* rem is even: number of (2,8) partitions for this rem */
        idx_t k = rem >> 1;                     /* k = rem/2 */
        seq_t term = (seq_t)(k / 4ULL) + 1ULL;    /* floor(k/4)+1 */
        sum += term;
    }
    return sum;
}
