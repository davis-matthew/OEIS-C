/*
 * A234307  a(n) = Sum_{i=1..n} gcd(2*n-i, i).
 * https://oeis.org/A234307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} gcd(2*n-i, i)
 * Range:     u8 n <= 44, u16 n <= 3149, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Fast direct summation over i = 1..n */
    seq_t result = 0;
    seq_t two_n = (seq_t)n + (seq_t)n;        /* 2*n as seq_t */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i_val  = (seq_t)i;             /* i as seq_t */
        seq_t other  = two_n - i_val;       /* 2*n - i as seq_t */
        result += gcd(i_val, other);        /* gcd from utils/gcd.h */
    }
    return result;
}
