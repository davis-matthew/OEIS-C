/*
 * A183981  1/4 the number of (n+1) X 5 binary arrays with all 2 X 2 subblock
 *          sums the same.
 * https://oeis.org/A183981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n/2-1) + 2^(n-1) + 12 (n even) * a(n) = 2^(n-1) +
 *            2^((n+1)/2) + 12 (n odd) * (valid for all n ≥ 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n < 1, return 0 for safety */
        return 0;

    if ((n & 1) == 0) {            /* even n */
        seq_t term1 = pow2(n / 2 - 1); /* 2^(n/2-1) */
        term1 = term1 * 3;           /* 3*2^(n/2-1) */
        seq_t term2 = pow2(n - 1);    /* 2^(n-1) */
        return term1 + term2 + 12;
    } else {                       /* odd n */
        seq_t term1 = pow2(n - 1);          /* 2^(n-1) */
        seq_t term2 = pow2((n + 1) / 2);    /* 2^((n+1)/2) */
        return term1 + term2 + 12;
    }
}
