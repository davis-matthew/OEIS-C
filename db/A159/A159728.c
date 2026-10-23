/*
 * A159728  Number of permutations of 4 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 2 local maxima.
 * https://oeis.org/A159728
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(121*35^(n-2) + 8*5^(n-2) - 24*n*5^(n-2))/9
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;        /* offset starts at 2 */
    }

    /* Special handling for 8‑bit seq_t to avoid intermediate overflow
       on the very first term (a(2) = 36).  For larger seq_t sizes
       the generic calculation fits safely. */
    if (sizeof(seq_t) == 1 && n == 2) {
        return (seq_t)36;
    }

    seq_t N   = (seq_t)n;
    seq_t exp = (seq_t)(n - 2);

    seq_t pow35 = big_pow((seq_t)35, exp);   /* 35^(n-2) */
    seq_t pow5  = big_pow((seq_t)5,  exp);   /* 5^(n-2)  */

    seq_t term = (seq_t)121 * pow35;        /* 121*35^(n-2) */
    term += (seq_t)8 * pow5;                /* +8*5^(n-2) */
    term -= (seq_t)24 * N * pow5;           /* -24*n*5^(n-2) */

    term = (seq_t)2 * N * term;             /* *2*n */
    term /= (seq_t)9;                       /* /9 */

    return term;
}
