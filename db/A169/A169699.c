/*
 * A169699  Total number of ON cells at stage n of two-dimensional 5-neighbor
 *          outer totalistic cellular automaton defined by "Rule 510".
 * https://oeis.org/A169699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0)=1; for n>0 let k=floor(log2(n)); let wt=popcount(n); a(n) =
 *            ((1<<(k+1))-1) * (1<<(1+wt)); if ((n+1)&n)==0 (i.e., n+1 is a
 *            power of 2) then a(n)+=1
 * Range:     u8 n <= 14, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* compute floor(log2(n)) */
    unsigned k = 0;
    uint64_t t = n;
    while (t >>= 1)
        ++k;

    /* binary weight of n */
    int wt = __builtin_popcountll(n);

    /* compute (2^{k+1}-1) * 2^{1+wt} */
    seq_t base = ((seq_t)1 << (k + 1)) - (seq_t)1;
    seq_t mult = ((seq_t)1 << (1 + wt));
    seq_t result = base * mult;

    /* add 1 if n+1 is a power of 2 (i.e., n = 2^k-1) */
    if (((n + 1) & n) == 0)
        result += (seq_t)1;

    return result;
}
