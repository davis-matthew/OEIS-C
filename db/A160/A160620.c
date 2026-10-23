/*
 * A160620  a(n) = Sum_{d|n} phi(n/d)^2*2^d.
 * https://oeis.org/A160620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d)^2 * 2^d
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t, idx_t are defined by the build environment. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

#define MAX_N 100000          /* limit for precomputation */

static bool        initialized = false;
static uint64_t    primes[MAX_N];      /* prime list */
static size_t      prime_cnt;

static idx_t       phi_arr[MAX_N + 1]; /* Euler totient values for 0..MAX_N */

/* ------------------------------------------------------------------ */
/* Helper: generate all divisors of a number from its prime factorization  */
static void gen_divisors(const idx_t *pfac,
                         const int *exp,
                         size_t idx,
                         size_t total,
                         idx_t cur,
                         idx_t *out,
                         size_t *cnt)
{
    if (idx == total) {
        out[(*cnt)++] = cur;
        return;
    }
    idx_t v = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divisors(pfac, exp, idx + 1, total, v, out, cnt);
        v *= pfac[idx];
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle offset n = 0 */
    if (n == 0) return (seq_t)0;

    /* Lazily pre‑compute primes and phi up to MAX_N */
    if (!initialized) {
        bool is_composite[MAX_N + 1] = {false};

        /* Sieve of Eratosthenes */
        for (size_t i = 2; i <= MAX_N; ++i) {
            if (!is_composite[i]) {
                primes[prime_cnt++] = (uint64_t)i;
                if (i * i <= MAX_N) {
                    for (size_t j = i * i; j <= MAX_N; j += i)
                        is_composite[j] = true;
                }
            }
        }

        /* Euler totient for all numbers <= MAX_N */
        for (size_t i = 0; i <= MAX_N; ++i)
            phi_arr[i] = (idx_t)i;          /* phi[i] = i initially */
        phi_arr[0] = 0;
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            uint64_t p = primes[pi];
            for (size_t j = p; j <= MAX_N; j += p)
                phi_arr[j] -= phi_arr[j] / (idx_t)p;
        }

        initialized = true;
    }

    /* ---------------------------------------------------------------- */
    /* Factor n: store prime factors and their exponents */
    idx_t temp = n;
    idx_t pfac[16];
    int    exp[16];
    size_t pf_cnt = 0;

    for (size_t pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= temp; ++pi) {
        uint64_t p = primes[pi];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            pfac[pf_cnt] = (idx_t)p;
            exp[pf_cnt]  = e;
            ++pf_cnt;
        }
    }
    if (temp > 1) {      /* remaining prime factor */
        pfac[pf_cnt] = temp;
        exp[pf_cnt]  = 1;
        ++pf_cnt;
    }

    /* ---------------------------------------------------------------- */
    /* Generate all divisors of n */
    idx_t divisors[256];
    size_t div_cnt = 0;
    gen_divisors(pfac, exp, 0, pf_cnt, (idx_t)1, divisors, &div_cnt);

    /* ---------------------------------------------------------------- */
    /* Sum over divisors */
    seq_t sum = (seq_t)0;
    idx_t original = n;
    for (size_t i = 0; i < div_cnt; ++i) {
        idx_t d = divisors[i];
        idx_t k = original / d;           /* n / d */
        seq_t phi_k  = (seq_t)phi_arr[k];
        seq_t phi_sq = phi_k * phi_k;
        seq_t pow2_d = pow2(d);            /* 2^d */
        sum += phi_sq * pow2_d;
    }

    return sum;
}
