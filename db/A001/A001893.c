/*
 * A001893  Number of permutations of (1,...,n) having n-3 inversions (n>=3).
 * https://oeis.org/A001893
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^(n-3)] Product_{i=1..n} (1-q^i)/(1-q); since n-3 < n,
 *            by Euler's pentagonal theorem a(n) = Sum_{j} (-1)^j *
 *            binomial(2n-4-g_j, n-1), where g_j = j(3j-1)/2 ranges over the
 *            generalized pentagonal numbers <= n-3. For fixed-width seq_t the
 *            same coefficient is computed by an additive DP (row...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   a001893_big, a001893_bigmode, a001893_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a001893_big.h"
#include "../utils/a001893_bigmode.h"
#include "../utils/a001893_fixed.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (a001893_bigmode()) return a001893_big(n);
    return a001893_fixed(n);
}
