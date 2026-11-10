/*
 * A077663  Sum of terms of n-th row of A077661.
 * https://oeis.org/A077663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the n smallest numbers not coprime to n (a(1)=1);
 *            computed by merging multiples of the prime divisors of n
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t ps[20];
    uint64_t cur[20];
    size_t k = 0;
    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            ps[k++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) ps[k++] = t;
    for (size_t i = 0; i < k; ++i) cur[i] = ps[i];

    seq_t sum = 0;
    uint64_t cnt = 0;
    while (cnt < (uint64_t)n) {
        uint64_t mn = cur[0];
        for (size_t i = 1; i < k; ++i)
            if (cur[i] < mn) mn = cur[i];
        sum += (seq_t)mn;
        ++cnt;
        for (size_t i = 0; i < k; ++i)
            if (cur[i] == mn) cur[i] += ps[i];
    }
    return sum;
}
