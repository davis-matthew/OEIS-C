/*
 * A054617  a(n) = Sum_{d|n} phi(d)*10^(n/d).
 * https://oeis.org/A054617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 10^gcd(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t g = gcd((seq_t)n, (seq_t)k);   /* compute gcd(n,k) */
        sum += pow10((idx_t)g);              /* add 10^g */
    }
    return sum;
}
