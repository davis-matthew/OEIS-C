/*
 * A127319  a(n) = binomial(n, sd) mod n, with sd = sum of decimal digits of
 *          n.
 * https://oeis.org/A127319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = binomial(n, A007953(n)) mod n
 * Range:     u8 n <= 369, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_mul.h"
#include "../utils/build_primes.h"

static inline int digit_sum_idx(idx_t n)
{
    int sum = 0;
    while (n > 0) {
        sum += (int)(n % 10);
        n /= 10;
    }
    return sum;
}

/* integer square root of a uint64_t, using __uint128_t to avoid overflow */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = ~((idx_t)0) >> 1;        /* high = 2^63-1 */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* static cache for primes up to a certain limit */
static idx_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static idx_t  prime_limit = 0;

/* get primes up to 'limit', rebuilding cache if needed */
static void ensure_primes(idx_t limit)
{
    if (limit > prime_limit) {
        if (prime_list) free(prime_list);
        build_primes(limit, &prime_list, &prime_cnt);
        prime_limit = limit;
    }
}

/* add or update a prime exponent in the map */
static void add_factor(uint64_t *primes, long long *exps, size_t *cnt,
                       uint64_t p, long long e)
{
    for (size_t i = 0; i < *cnt; ++i) {
        if (primes[i] == p) {
            exps[i] += e;
            return;
        }
    }
    /* new prime */
    if (*cnt + 1 > 128) {
        /* should not happen for the given constraints */
        return;
    }
    primes[*cnt] = p;
    exps[*cnt]  = e;
    ++(*cnt);
}

/* fast modular exponentiation for uint64_t */
static inline uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1) result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* unused, offset 1 */
    int sd = digit_sum_idx(n);
    if (sd == 0 || sd == n) {
        return (seq_t)(1 % (seq_t)n);
    }
    if (sd > (int)n) return 0;      /* safety, though sd <= n for n>=1 */

    /* prepare primes up to sqrt(n) */
    idx_t limit = isqrt_u64(n);
    ensure_primes(limit);
    uint64_t primes[128] = {0};
    long long exps[128] = {0};
    size_t cnt = 0;

    /* factor denominators 1..sd */
    for (idx_t d = 1; d <= (idx_t)sd; ++d) {
        idx_t x = d;
        for (size_t i = 0; i < prime_cnt && prime_list[i]*prime_list[i] <= x; ++i) {
            uint64_t p = prime_list[i];
            long long e = 0;
            while (x % p == 0) {
                ++e;
                x /= p;
            }
            if (e) add_factor(primes, exps, &cnt, p, -e);
        }
        if (x > 1) {
            add_factor(primes, exps, &cnt, (uint64_t)x, -1);
        }
    }

    /* factor numerators n-sd+1 .. n */
    for (idx_t m = n - (idx_t)sd + 1; m <= n; ++m) {
        idx_t x = m;
        for (size_t i = 0; i < prime_cnt && prime_list[i]*prime_list[i] <= x; ++i) {
            uint64_t p = prime_list[i];
            long long e = 0;
            while (x % p == 0) {
                ++e;
                x /= p;
            }
            if (e) add_factor(primes, exps, &cnt, p, e);
        }
        if (x > 1) {
            add_factor(primes, exps, &cnt, (uint64_t)x, 1);
        }
    }

    uint64_t mod = (uint64_t)n;
    uint64_t r    = 1 % mod;

    for (size_t i = 0; i < cnt; ++i) {
        long long e = exps[i];
        if (e <= 0) continue;
        uint64_t pmod = primes[i] % mod;
        uint64_t pe   = pow_mod_uint64(pmod, (uint64_t)e, mod);
        r = mod_mul(r, pe, mod);
    }

    return (seq_t)r;
}
