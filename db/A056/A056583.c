/*
 * A056583  Solution to a(n)^(n^2/a(n)) = gcd(n^n, Product_{k<n} k^k) where
 *          a(n) and n^2/a(n) are integers, or 0 if no such integers exist.
 * https://oeis.org/A056583
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the unique integer a such that a^(n^2/a) =
 *            gcd(n^n,∏_{k<n}k^k); a and n^2/a are integers, otherwise a(n)=0
 * Range:     u8 n <= 15, u16 n <= 1850, u32 n <= 9611, u64 n <= 9611,
 *            u128 n <= 9611, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static void dfs_divs(const uint32_t *primes, const int *orig_exp, size_t nprimes,
                     size_t idx, seq_t d, int *expCur, seq_t n_sq,
                     const seq_t *g_exp, seq_t *ans, bool *found)
{
    if (*found) return;
    if (idx == nprimes) {
        seq_t e = n_sq / d;
        for (size_t i = 0; i < nprimes; ++i) {
            seq_t cand = (seq_t)expCur[i] * e;
            if (cand != g_exp[i]) return;
        }
        *ans = d;
        *found = true;
        return;
    }
    uint32_t p = primes[idx];
    seq_t d_local = d;
    int max_e = 2 * orig_exp[idx];
    for (int e_exp = 0; e_exp <= max_e; ++e_exp) {
        expCur[idx] = e_exp;
        dfs_divs(primes, orig_exp, nprimes, idx + 1, d_local, expCur,
                 n_sq, g_exp, ans, found);
        if (*found) return;
        d_local *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Build a small prime sieve once */
    static bool primes_inited = false;
    static uint32_t primes[20000];
    static size_t prime_cnt = 0;
    if (!primes_inited) {
        const int LIM = 20000;
        bool sieve[LIM + 1];
        for (int i = 0; i <= LIM; ++i) sieve[i] = true;
        sieve[0] = sieve[1] = false;
        for (int i = 2; (long long)i * i <= LIM; ++i)
            if (sieve[i])
                for (int j = i * i; j <= LIM; j += i)
                    sieve[j] = false;
        for (int i = 2; i <= LIM; ++i)
            if (sieve[i]) primes[prime_cnt++] = i;
        primes_inited = true;
    }

    /* Factor n */
    uint64_t temp = n;
    uint32_t fac_pr[8];
    int fac_ex[8];
    size_t fac_cnt = 0;
    for (size_t i = 0; i < prime_cnt &&
            (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        if (temp % primes[i] == 0) {
            int e = 0;
            while (temp % primes[i] == 0) {
                temp /= primes[i];
                ++e;
            }
            fac_pr[fac_cnt] = primes[i];
            fac_ex[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (temp > 1) {
        fac_pr[fac_cnt] = (uint32_t)temp;
        fac_ex[fac_cnt] = 1;
        ++fac_cnt;
    }

    /* Compute exponent of each prime in gcd */
    seq_t sum_exp[8];
    seq_t g_exp[8];
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac_pr[i];
        seq_t total = 0;
        seq_t power = (seq_t)p;
        seq_t n_minus1 = (seq_t)(n - 1);
        while (power <= n_minus1) {
            seq_t m = n_minus1 / power;
            seq_t term = power * m * (m + 1) / 2;
            total += term;
            power *= (seq_t)p;
        }
        sum_exp[i] = total;
        seq_t candidate = (seq_t)n * (seq_t)fac_ex[i];
        g_exp[i] = (total < candidate) ? total : candidate;
    }

    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* Enumerate divisors of n^2 and find the one satisfying the condition */
    static bool found = false;
    static seq_t answer = 0;
    found = false;
    answer = 0;
    int cur_exp[8] = {0};

    dfs_divs(fac_pr, fac_ex, fac_cnt, 0, (seq_t)1,
             cur_exp, n_sq, g_exp, &answer, &found);

    return found ? answer : (seq_t)0;
}
