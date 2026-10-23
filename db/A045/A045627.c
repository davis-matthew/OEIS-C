/*
 * A045627  Number of 2n-bead black-white reversible strings with n black
 *          beads and fundamental period 2n.
 * https://oeis.org/A045627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (P(n)+S(n))/2 * * P(n) = ∑_{d|n} μ(d) · C(2n/d, n/d)
 *            (primitive necklaces) * S(n) = ∑_{d|n, n/d even} μ(d) · C(n/d,
 *            (n/d)/2) * (contributions of symmetric necklaces) * For odd n,
 *            S(n)=0. The quotient is an integer
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* ----  helper tables  --------------------------------------------------- */
#define MAX_N   20000              /* must be ≥ 2·max(n) that will be queried */

static int  mu[MAX_N + 1];
static bool is_composite[MAX_N + 1];
static idx_t primes[MAX_N];
static size_t prime_cnt = 0;
static bool mu_inited = false;

/* ----  Möbius + prime sieve  ------------------------------------------- */
static void init_mu(void)
{
    if (mu_inited) return;
    mu_inited = true;
    mu[1] = 1;
    for (idx_t i = 2; i <= MAX_N; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > MAX_N) break;
            is_composite[v] = true;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            }
            mu[v] = -mu[i];
        }
    }
}

/* ----  divisor enumeration  --------------------------------------------- */
static void get_divisors(idx_t n, idx_t *divs, size_t *cnt)
{
    *cnt = 0;
    if (n == 0) return;
    idx_t tmp = n;
    struct { idx_t p; int e; } fac[16];
    size_t fcnt = 0;
    for (size_t i = 0; i < prime_cnt && primes[i]*primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            int e = 0;
            while (tmp % primes[i] == 0) { tmp /= primes[i]; ++e; }
            fac[fcnt].p = primes[i];
            fac[fcnt].e = e;
            ++fcnt;
        }
    }
    if (tmp > 1) {
        fac[fcnt].p = tmp;
        fac[fcnt].e = 1;
        ++fcnt;
    }

    divs[0] = 1; *cnt = 1;
    for (size_t i = 0; i < fcnt; ++i) {
        idx_t p = fac[i].p;
        int e  = fac[i].e;
        size_t cur = *cnt;
        idx_t mult = 1;
        for (int k = 1; k <= e; ++k) {
            mult *= p;
            for (size_t j = 0; j < cur; ++j) {
                divs[*cnt] = divs[j] * mult;
                ++(*cnt);
            }
        }
    }
}

/* ----  binomial coefficient  ------------------------------------------- */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* ----  main sequence function  ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    init_mu();

    /* divisors of n */
    idx_t divs[256];
    size_t divcnt;
    get_divisors(n, divs, &divcnt);

    /* P(n) = Σ μ(d) * C(2n/d, n/d) */
    seq_t P = (seq_t)0;
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d = divs[i];
        int m = mu[d];
        if (m == 0) continue;
        idx_t nd = (idx_t)(2 * n) / d;
        idx_t k  = n / d;
        seq_t term = binom(nd, k);
        if (m > 0) P += term; else P -= term;
    }

    /* S(n) = Σ μ(d) * C(n/d, (n/d)/2)  (only when n/d is even) */
    seq_t S = (seq_t)0;
    if ((n & 1ULL) == 0) {
        for (size_t i = 0; i < divcnt; ++i) {
            idx_t d = divs[i];
            int m = mu[d];
            if (m == 0) continue;
            idx_t mdiv = n / d;
            if ((mdiv & 1ULL) != 0) continue;   /* mdiv must be even */
            seq_t term = binom(mdiv, mdiv / 2);
            if (m > 0) S += term; else S -= term;
        }
    }

    if ((n & 1ULL) == 0)
        return (P + S) / (seq_t)2;
    else
        return P / (seq_t)2;
}
