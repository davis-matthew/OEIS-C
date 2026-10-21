/*
 * A227487  The hyper-Wiener index of the nanostar dendrimer defined
 *          pictorially as G[n] in the M. H. Khalifeh et al. reference.
 * https://oeis.org/A227487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            -6011+2^n*(-111166-5960*n+7000*n^2)+4^n*(151432-35200*n+40000*n^2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    int64_t poly1 = -111166 - 5960LL * (int64_t)n + 7000LL * (int64_t)n * (int64_t)n;
    int64_t poly2 = 151432 - 35200LL * (int64_t)n + 40000LL * (int64_t)n * (int64_t)n;

    seq_t p2 = pow2(n);
    seq_t p4 = pow2(2ULL * n);

    seq_t abs_poly1 = poly1 >= 0 ? (seq_t)poly1 : (seq_t)(-poly1);
    seq_t abs_poly2 = poly2 >= 0 ? (seq_t)poly2 : (seq_t)(-poly2);

    seq_t t1 = p2 * abs_poly1;
    seq_t t2 = p4 * abs_poly2;

    seq_t result = t2;
    if (poly1 >= 0)
        result += t1;
    else
        result -= t1;

    result -= (seq_t)6011;
    return result;
}
