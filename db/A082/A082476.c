/*
 * A082476  a(n) = Sum_{d|n} mu(d)^2*tau(d)^2.
 * https://oeis.org/A082476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = 5^omega(n) where omega(n) is the number of distinct prime
 *            factors of n
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int count_distinct_primes(uint64_t n)
{
    int cnt = 0;
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) cnt++;   /* remaining prime factor */
    return cnt;
}

seq_t a(idx_t n)
{
    uint64_t num = (uint64_t)n;            /* cast index to 64‑bit unsigned */
    int omega = count_distinct_primes(num);
    seq_t res = 1;
    for (int i = 0; i < omega; ++i) res *= (seq_t)5;
    return res;
}
