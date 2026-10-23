/*
 * A023137  Number of cycles of function f(x) = 5x mod n.
 * https://oeis.org/A023137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | m} phi(d) / ord_5(d), where m = n with all
 *            factors of 5 removed. ord_5(d) is the multiplicative order of 5
 *            modulo d
 * Range:     u8 n <= 996, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

#define MAX_N      10000      /* largest n that may be requested */
#define MAX_PRIMES 1229       /* number of primes <= 10000 */

/* cache of primes up to MAX_N */
static bool          primes_ready = false;
static idx_t         primes_arr[MAX_PRIMES];
static size_t        prime_cnt = 0;

/* generate primes up to MAX_N (called once) */
static void init_primes(void)
{
    if (primes_ready) return;
    static bool is_comp[MAX_N + 1];
    is_comp[0] = is_comp[1] = true;
    for (idx_t i = 2; i * i <= MAX_N; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= MAX_N; j += i)
                is_comp[j] = true;
        }
    }
    prime_cnt = 0;
    for (idx_t i = 2; i <= MAX_N; ++i)
        if (!is_comp[i]) primes_arr[prime_cnt++] = i;
    primes_ready = true;
}

/* factorize n into primes/out arrays (max 128 factors). */
static void factorize(idx_t n, idx_t primes_out[128], idx_t exps_out[128], size_t *cnt_out)
{
    *cnt_out = 0;
    if (n <= 1) return;
    init_primes();
    for (size_t i = 0; i < prime_cnt && primes_arr[i] * primes_arr[i] <= n; ++i) {
        idx_t p = primes_arr[i];
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes_out[*cnt_out] = p;
            exps_out[*cnt_out] = e;
            ++(*cnt_out);
        }
    }
    if (n > 1) {                 /* remaining prime factor */
        primes_out[*cnt_out] = n;
        exps_out[*cnt_out] = 1;
        ++(*cnt_out);
    }
}

/* recursive helper to generate all divisors of n. */
static void gen_divs_rec(size_t idx, idx_t cur,
                         const idx_t primes[], const idx_t exps[], size_t cnt,
                         idx_t divs[], size_t *divcnt)
{
    if (idx == cnt) {
        divs[*divcnt] = cur;
        ++(*divcnt);
        return;
    }
    idx_t p = primes[idx];
    idx_t mult = 1;
    for (idx_t e = 0; e <= exps[idx]; ++e) {
        gen_divs_rec(idx + 1, cur * mult, primes, exps, cnt, divs, divcnt);
        mult *= p;
    }
}

static void gen_divisors(idx_t n, idx_t divs[], size_t *divcnt)
{
    idx_t primes[128], exps[128];
    size_t cnt;
    factorize(n, primes, exps, &cnt);
    *divcnt = 0;
    gen_divs_rec(0, 1, primes, exps, cnt, divs, divcnt);
}

/* Euler’s totient of n */
static seq_t phi_of(idx_t n)
{
    if (n == 1) return 1;
    idx_t primes[128], exps[128];
    size_t cnt;
    factorize(n, primes, exps, &cnt);
    seq_t result = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        result = result / p * (p - 1);
    }
    return result;
}

/* multiplicative order of 5 modulo d (d >= 1, gcd(5,d)=1) */
static seq_t order5(idx_t d)
{
    if (d == 1) return 1;
    seq_t phi_d = phi_of(d);

    /* factor phi_d */
    idx_t primes_phi[128], exps_phi[128];
    size_t cnt_phi;
    factorize((idx_t)phi_d, primes_phi, exps_phi, &cnt_phi);

    seq_t ord = phi_d;
    for (size_t i = 0; i < cnt_phi; ++i) {
        idx_t p = primes_phi[i];
        while (ord % p == 0) {
            seq_t cand = ord / (seq_t)p;
            seq_t r = mod_pow((seq_t)5, (idx_t)cand, (seq_t)d);
            if (r == 1)
                ord = cand;
            else
                break;
        }
    }
    return ord;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    /* remove all factors of 5 */
    idx_t m = n;
    while ((m % 5ULL) == 0ULL) m /= 5ULL;
    if (m == 1) return 1;          /* only 0 cycle in this case */

    /* generate all divisors of m */
    idx_t divs[256];
    size_t divcnt;
    gen_divisors(m, divs, &divcnt);

    seq_t total = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d = divs[i];
        seq_t phi_d = phi_of(d);
        seq_t ord = order5(d);
        total += phi_d / ord;
    }
    return total;
}
