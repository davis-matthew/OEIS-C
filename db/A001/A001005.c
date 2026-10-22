/*
 * A001005  Number of ways of partitioning n points on a circle into subsets
 *          only of sizes 2 and 3.
 * https://oeis.org/A001005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} C(n,k-1)*C(k,n-2k)/k, computed via
 *            the equivalent P-recurrence (arbitrary precision):
 *            2*(n+1)*(2*n+3)*(26*n+1)*a(n) = -(n-1)*(26*n^2+53*n+18)*a(n-1) +
 *            6*(n-1)*(78*n^2+42*n-25)*a(n-2) +
 *            31*(n-2)*(n-1)*(26*n+27)*a(n-3), a(0) = 1, a(1) = 0; and for
 *            fixed-width seq_t via...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 53, u128 n <= 100,
 *            big any n
 * Helpers:   a001005_dp, a001005_fixed, a001005_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a001005_dp.h"
#include "../utils/a001005_fixed.h"
#include "../utils/a001005_rec.h"

seq_t a(idx_t n) {
    if (a001005_fixed()) return a001005_dp(n);
    return a001005_rec(n);
}
