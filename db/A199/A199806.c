/*
 * A199806  Alternating LCM-sum: a(n) = Sum_{k=1..n} (-1)^(k-1)*lcm(k,n).
 * https://oeis.org/A199806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1..n} (-1)^(k-1) * lcm(k,n)
 * Range:     i8 n <= 11, i16 n <= 63, i32 n <= 2367, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t g  = gcd(kk, nn);                 /* gcd from utils */
        seq_t l  = (kk / g) * nn;                /* lcm(k,n)   */
        if ((k & 1) == 0)                       /* k even → sign negative */
            l = -l;
        sum += l;
    }
    return sum;
}
