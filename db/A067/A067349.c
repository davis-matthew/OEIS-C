/*
 * A067349  Number of divisors of sigma(n)+phi(n).
 * https://oeis.org/A067349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A000010(n) + A000203(n))
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 110 terms through n = 8215 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32, num_divisors, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of divisors of sigma(n) + phi(n) */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/num_divisors.h"
#include "../utils/totient.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Generic divisor count for arbitrary-precision seq_t values */
static seq_t count_divisors_generic(seq_t x)
{
    if (x <= 1) return 1;
    seq_t sqrt_x = floor_sqrt(x);
    idx_t limit = (idx_t)sqrt_x;

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t count = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* stop if p^2 > current x */
        if ((seq_t)p * (seq_t)p > x) break;
        unsigned exp = 0;
        while (x % (seq_t)p == 0) {
            x /= (seq_t)p;
            ++exp;
        }
        if (exp) count *= (seq_t)(exp + 1);
    }
    if (x > 1) count *= 2;      /* remaining prime factor */
    free(primes);
    return count;
}

seq_t a(idx_t n)
{
    seq_t sigma_val = sigma_of(n);
    seq_t phi_val   = totient(n);
    seq_t sum = sigma_val + phi_val;

    if ((seq_t)UINT64_MAX >= sum) {          /* sum fits into uint64_t */
        uint64_t m = (uint64_t)sum;
        uint64_t d = num_divisors(m);
        return (seq_t)d;
    }
    return count_divisors_generic(sum);
}
