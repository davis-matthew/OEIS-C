/*
 * A057443  For the numbers k that can be expressed as k = w + x = y*z with
 *          w*x = y^3 + z^3 where w, x, y, and z are all positive integers,
 *          this sequence gives the corresponding values of w*x.
 * https://oeis.org/A057443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating k and testing all divisor pairs (y,z)
   for each k where k = y*z and w*x = y³ + z³ with
   w+x = k and w*x = y³+z³. Values w*x are stored in the sequence. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static idx_t *spf = NULL;
static idx_t spf_limit = 0;

static void init_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    spf = (idx_t *)realloc(spf, (size_t)(limit + 1) * sizeof(idx_t));
    if (!spf) exit(1);
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i > limit / i) continue;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    spf_limit = limit;
}

static void factorize_k(idx_t n, idx_t primes[], uint8_t exps[], size_t *cnt)
{
    size_t i = 0;
    while (n > 1) {
        idx_t p = spf[n];
        if (p == 0) p = n;
        uint8_t e = 0;
        while (n % p == 0) { n /= p; ++e; }
        primes[i] = p;
        exps[i] = e;
        ++i;
    }
    *cnt = i;
}

static bool check_pair(idx_t y, idx_t z, seq_t *out)
{
    seq_t wx = (seq_t)y * (seq_t)y * (seq_t)y + (seq_t)z * (seq_t)z * (seq_t)z;
    seq_t k  = (seq_t)y * (seq_t)z;
    seq_t tmp1 = k * k;
    seq_t fourwx = 4 * wx;
    if (tmp1 < fourwx) return false;
    seq_t D = tmp1 - fourwx;
    seq_t sqrtD = floor_sqrt(D);
    if (sqrtD * sqrtD != D) return false;
    if (((k - sqrtD) & (seq_t)1) != 0) return false;
    *out = wx;
    return true;
}

static bool dfs_divisors(int idx, seq_t cur,
                         const idx_t *primes, const uint8_t *exps,
                         int prime_cnt, idx_t k, seq_t *out)
{
    if (idx == prime_cnt) {
        idx_t y = (idx_t)cur;
        idx_t z = k / y;
        return check_pair(y, z, out);
    }
    idx_t p = primes[idx];
    seq_t mul = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        if (dfs_divisors(idx + 1, cur * mul, primes, exps,
                         prime_cnt, k, out))
            return true;
        mul *= (seq_t)p;
    }
    return false;
}

static seq_t *seq_values = NULL;
static size_t values_cap = 0;
static size_t values_cnt = 0;
static idx_t last_k = 0;

static void ensure_n(idx_t n)
{
    if (!seq_values) {
        values_cap = 64;
        seq_values = (seq_t *)malloc(values_cap * sizeof(seq_t));
        if (!seq_values) exit(1);
    }
    const idx_t INITIAL_SPF = 200000;
    if (spf_limit == 0) init_spf(INITIAL_SPF);

    idx_t k = last_k;
    while (values_cnt < (size_t)n) {
        ++k;
        if (k > spf_limit) init_spf(k * 2U);

        idx_t primes[32];
        uint8_t exps[32];
        size_t prime_cnt;
        factorize_k(k, primes, exps, &prime_cnt);

        seq_t wx = 0;
        if (dfs_divisors(0, (seq_t)1, primes, exps,
                         (int)prime_cnt, k, &wx)) {
            if (values_cnt == values_cap) {
                values_cap *= 2;
                seq_values = (seq_t *)realloc(seq_values,
                                              values_cap * sizeof(seq_t));
                if (!seq_values) exit(1);
            }
            seq_values[values_cnt++] = wx;
        }
    }
    last_k = k;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_n(n);
    return seq_values[n - 1];
}
