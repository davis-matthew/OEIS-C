/*
 * A057789  a(n) = Sum_{k = 1..n, gcd(k,n)=1} k*(n-k).
 * https://oeis.org/A057789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k = 1..n, gcd(k,n)=1} k*(n-k)
 * Range:     u8 n <= 12, u16 n <= 78, u32 n <= 2956, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t N = (seq_t)n;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t K = (seq_t)k;
        if (gcd(K, N) == 1)
            sum += K * (N - K);
    }
    return sum;
}
