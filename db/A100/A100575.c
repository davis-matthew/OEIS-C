/*
 * A100575  Half the number of permutations of 0..n with exactly two maxima.
 * https://oeis.org/A100575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 4^(n-1) - (n+1)*2^(n-2) for n >= 2, a(0)=0, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t exp4 = 2 * (n - 1);      /* 4^(n-1) = 2^(2*(n-1)) */
    seq_t pow4 = pow2(exp4);
    seq_t pow2n2 = pow2(n - 2);    /* 2^(n-2) */
    seq_t term = pow4 - (seq_t)(n + 1) * pow2n2;
    return term;
}
