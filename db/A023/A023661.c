/*
 * A023661  Convolution of odd numbers and A014306.
 * https://oeis.org/A023661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - (2n+1)*K + 2*C(K+3,4) where K is the largest
 *            integer * such that K*(K+1)*(K+2)/6 <= n. Here C denotes the
 *            binomial coefficient
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   tetra_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/tetra_n.h"
/* This implements the convolution of odd numbers with sequence A014306
 * (indicator of tetrahedral numbers).  */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard for zero */

    seq_t ns = (seq_t)n;                  /* n as seq_t for arithmetic */

    /* Find K: the largest k with T(k) <= n.  Use exponential search
       then binary search.  */
    idx_t low  = 1;
    idx_t high = 1;
    while (tetra_n(high) <= ns) {
        if (high > (UINT64_MAX >> 1)) break;   /* prevent infinite loop */
        high <<= 1;
    }

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (tetra_n(mid) <= ns)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t K = low;

    seq_t kk = (seq_t)K;

    /* n^2 */
    seq_t ns2 = ns * ns;

    /* (2n+1)*K */
    seq_t term1 = ((seq_t)2 * ns + (seq_t)1) * kk;

    /* 2*C(K+3,4) where C(a,b) = a!/(b!(a-b)!) */
    seq_t a1 = kk + (seq_t)3;
    seq_t a2 = kk + (seq_t)2;
    seq_t a3 = kk + (seq_t)1;
    seq_t a4 = kk;
    seq_t binom = a1 * a2 * a3 * a4 / (seq_t)24;   /* 24 = 4! */

    /* Final formula */
    seq_t result = ns2 - term1 + (seq_t)2 * binom;
    return result;
}
