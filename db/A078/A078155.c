/*
 * A078155  A078152(n!).
 * https://oeis.org/A078155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A078152(n!) = A055086(n!) - A000005(n!), where A055086(N)
 *            = floor(sqrt(4N+1)) - 1 = number of distinct floor(N/j);
 *            computed from r = isqrt(N), and d(n!) from Legendre exponents
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 33, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 773 (2026-11-07)
 * Helpers:   isqrt_seq, isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u128.h"
#include "../utils/isqrt_seq.h"

static seq_t A078155_u128_to_seq(unsigned __int128 x)
{
    seq_t hi = (seq_t)(uint64_t)(x >> 64);
    seq_t lo = (seq_t)(uint64_t)x;
    hi *= 4294967296u;
    hi *= 4294967296u;
    return hi + lo;
}

static seq_t A078155_divisors_of_factorial(idx_t n)
{
    seq_t prod = 1;
    for (idx_t p = 2; p <= n; ++p) {
        bool prime = true;
        for (idx_t q = 2; q * q <= p; ++q) {
            if (p % q == 0) { prime = false; break; }
        }
        if (!prime) continue;
        idx_t e = 0;
        idx_t t = n;
        while (t > 0) {
            t /= p;
            e += t;
        }
        prod *= (seq_t)(e + 1);
    }
    return prod;
}

seq_t a(idx_t n)
{
    seq_t cnt;
    if (n <= 34) {
        unsigned __int128 N = 1;
        for (idx_t i = 2; i <= n; ++i) N *= i;
        unsigned __int128 r = isqrt_u128(N);
        unsigned __int128 c = (N >= r * (r + 1)) ? 2 * r : 2 * r - 1;
        cnt = A078155_u128_to_seq(c);
    } else {
        seq_t N = 1;
        for (idx_t i = 2; i <= n; ++i) N *= (seq_t)i;
        seq_t r = isqrt_seq(N);
        if (N >= r * (r + 1)) cnt = 2 * r;
        else cnt = 2 * r - 1;
    }
    return cnt - A078155_divisors_of_factorial(n);
}
