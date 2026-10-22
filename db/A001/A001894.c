/*
 * A001894  Number of permutations of {1,...,n} having n-4 inversions (n>=4).
 * https://oeis.org/A001894
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = I_n(n-4), where sum_k I_n(k) x^k = Product_{i=1..n}
 *            (1-x^i)/(1-x). For k <= n, Euler's pentagonal theorem gives
 *            I_n(k) = Sum_{e generalized pentagonal, e<=k} s(e) * C(n-1+k-e,
 *            k-e), with e = j(3j-1)/2 and j(3j+1)/2, sign s(e) = (-1)^j
 *            (s(0)=1). Small n use the product DP directly
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Helpers:   a001894_big, a001894_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001894_big.h"
#include "../utils/a001894_small.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if (n <= 200) return a001894_small(n);
    return a001894_big(n);
}
