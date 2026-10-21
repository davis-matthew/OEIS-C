/*
 * A055891  CIK (necklace, indistinct, unlabeled) transform of powers of 2.
 * https://oeis.org/A055891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} φ(n/d) * 2^d * (2^d - 1) (with a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* Smallest prime factor sieve cache */
static uint64_t *spf_cache = NULL;
static size_t spf_cap = 0;

/* Ensure spf_cache is built up to at least n (inclusive). */
static void ensure_spf_cache(idx_t n)
{
    if ((size_t)n + 1 <= spf_cap) return;

    size_t new_cap = (size_t)n + 1;
    uint64_t *new_arr = (uint64_t *)realloc(spf_cache, new_cap * sizeof(uint64_t));
    if (!new_arr) { /* Allocation failed – abort. */ return; }
    for (size_t i = 0; i < new_cap; ++i) new_arr[i] = 0;

    if (new_cap > 0) new_arr[0] = 0;
    if (new_cap > 1) new_arr[1] = 0;

    for (size_t i = 2; i < new_cap; ++i) {
        if (new_arr[i] == 0) {
            new_arr[i] = i;
            if (i * i < new_cap) {
                for (size_t j = i * i; j < new_cap; j += i)
                    if (new_arr[j] == 0) new_arr[j] = i;
            }
        }
    }
    spf_cache = new_arr;
    spf_cap = new_cap;
}

/* Compute φ(m) using the spf cache (m fits in uint64_t). */
static seq_t phi_from_spf(uint64_t m)
{
    seq_t result = (seq_t)m;
    uint64_t temp = m;
    while (temp > 1) {
        uint64_t p = spf_cache[temp];
        while (temp % p == 0) temp /= p;
        result = result - result / p;
    }
    return result;
}

/* Add contribution for a divisor d of n. */
static void add_divisor_contribution(uint64_t d, idx_t n, seq_t *sum)
{
    uint64_t m = n / d;
    seq_t phi_val = phi_from_spf(m);
    seq_t pow2_d   = pow2((idx_t)d);
    seq_t term = phi_val * pow2_d * (pow2_d - (seq_t)1);
    *sum += term;
}

/* Recursive enumeration of all divisors from the prime factorization of n. */
static void enumerate_divisors(const uint64_t *primes, const uint32_t *exps,
                               int pf_cnt, int step, uint64_t current,
                               idx_t n, seq_t *sum)
{
    if (step == pf_cnt) {
        add_divisor_contribution(current, n, sum);
        return;
    }
    uint64_t p = primes[step];
    uint64_t val = current;
    for (uint32_t e = 0; e <= exps[step]; ++e) {
        enumerate_divisors(primes, exps, pf_cnt, step + 1, val, n, sum);
        val *= p;
    }
}

/* Main function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Build SPF sieve up to n. */
    ensure_spf_cache(n);

    /* Factorize n. */
    uint64_t N = n;
    uint64_t primes[20];
    uint32_t exps[20];
    int pf_cnt = 0;
    while (N > 1) {
        uint64_t p = spf_cache[N];
        uint32_t e = 0;
        while (N % p == 0) {
            N /= p;
            ++e;
        }
        primes[pf_cnt]  = p;
        exps[pf_cnt]   = e;
        ++pf_cnt;
    }

    seq_t sum = 0;
    enumerate_divisors(primes, exps, pf_cnt, 0, 1, n, &sum);
    return sum / (seq_t)n;
}
