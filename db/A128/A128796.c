/*
 * A128796  a(n) = n*(n-1)*2^n.
 * https://oeis.org/A128796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*2^n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 23, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 962 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;
    }

    seq_t tn  = (seq_t)n;
    seq_t tn1 = (seq_t)(n - 1);
    seq_t res = tn * tn1;
    res *= pow2(n);   /* 2^n */
    return res;
}
