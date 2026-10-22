/*
 * A007344  Number of homogeneous primitive partition identities of degree 6
 *          with largest part n.
 * https://oeis.org/A007344
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_s C(c3(s),2) - n*Sum_s C(c2(s),2), where ck(s) =
 *            number of k-multisets of {1..n} with sum s (coefficients of the
 *            Gaussian binomial [n+k-1,k]_q). This counts unordered pairs of
 *            disjoint equal-sum 3-multisets of {1..n}, i.e. homogeneous
 *            primitive partition identities a1+a2+a3 = b1+b2+b3...
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   gauss_coeffs, sum_pairs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gauss_coeffs.h"
#include "../utils/sum_pairs.h"

seq_t a(idx_t n)
{
    long long len = 3 * (long long)n + 8;
    long long *p = (long long *)malloc((size_t)len * sizeof(long long));
    long long d;
    seq_t s2, s3;
    d = gauss_coeffs(p, len, n, 2);
    s2 = sum_pairs(p, d);
    d = gauss_coeffs(p, len, n, 3);
    s3 = sum_pairs(p, d);
    free(p);
    return s3 - (seq_t)n * s2;
}
