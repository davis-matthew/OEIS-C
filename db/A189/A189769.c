/*
 * A189769  Number of one-sided prudent walks from (0,0) to (n,n), with 3+n
 *          east steps, 3 west steps and n north steps.
 * https://oeis.org/A189769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = n*(n+1)*(n+2)/3 * binom(2*n-1, n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // offset is 1, but guard against n==0
    seq_t pref = (seq_t)n * (seq_t)(n + 1);
    pref *= (seq_t)(n + 2);
    pref /= 3;                     // n,(n+1),(n+2) contain a factor 3
    seq_t comb = binom(2 * n - 1, n - 1);
    return pref * comb;
}
