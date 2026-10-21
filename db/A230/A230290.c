/*
 * A230290  a(n) = Sum_{i=1..n} d(24*i+1) - Sum_{i=1..n} d(6*i+1), where d(n)
 *          = A000005(n).
 * https://oeis.org/A230290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} d(24*i+1) - Sum_{i=1..n} d(6*i+1)
 * Range:     u8 n <= 544, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(k) = number of positive divisors of k */

/* This implementation evaluates d(k) by trial division in O(sqrt(k)). */
/* It works for both the C and the C++ (big integer) builds and requires no dynamic memory. */

#include <stdint.h>

/* returns d(n) as a seq_t (converted from a 64‑bit count) */
static seq_t divisor_count_uint64(uint64_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t cnt = 1;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            cnt *= (exp + 1);
        }
    }
    if (tmp > 1) cnt *= 2;  /* remaining prime factor has exponent 1 */
    return (seq_t)cnt;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t sum24 = 0;
    seq_t sum6  = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t x24 = 24ULL * i + 1ULL;
        uint64_t x6  = 6ULL  * i + 1ULL;
        sum24 += divisor_count_uint64(x24);
        sum6  += divisor_count_uint64(x6);
    }
    return sum24 - sum6;
}
