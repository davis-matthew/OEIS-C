/*
 * A237524  Number of ordered ways to write n = i + j + k with 0 < i <= j <= k
 *          such that phi(i*j*k) is a cube, where phi(.) is Euler's totient
 *          function.
 * https://oeis.org/A237524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (i,j,k) with 0<i≤j≤k, i+j+k=n
 *            such that φ(i·j·k) is a perfect cube. The function precomputes
 *            smallest prime factors up to 10000 once and uses them to test
 *            every triple in O(n²) time
 * Range:     u8 n <= 629, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAXN 10000            /* maximum n handled by this implementation */

static bool initialized = false;

/* Smallest prime factor table – spf[x] = smallest prime that divides x */
static uint16_t spf[MAXN + 1];

/* Factorisation tables for integers up to MAXN.
   fac_len[n]    – number of distinct primes in n
   fac_pr[n][*]  – those primes
   fac_exp[n][*] – corresponding exponents
   We allocate room for at most 6 distinct primes (more than enough for ≤10000). */
static uint8_t fac_len[MAXN + 1];
static uint16_t fac_pr[MAXN + 1][6];
static uint8_t  fac_exp[MAXN + 1][6];

/* Factorisation tables for p-1, for each prime p ≤ MAXN. */
static uint8_t pf_len[MAXN + 1];          /* 0 for non‑primes */
static uint16_t pf_pr[MAXN + 1][6];
static uint8_t  pf_exp[MAXN + 1][6];

/* --------------------------------------------------------------- */
/* initialise tables – called once, on first call to a()           */
/* --------------------------------------------------------------- */
static void init_tables(void)
{
    /* sieve for smallest prime factor */
    for (int i = 0; i <= MAXN; ++i) spf[i] = i;
    for (int i = 2; i * i <= MAXN; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= MAXN; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    fac_len[0] = fac_len[1] = 0;
    for (int n = 2; n <= MAXN; ++n) {
        int x = n, l = 0;
        while (x > 1) {
            int p = spf[x], cnt = 0;
            while (x % p == 0) { x /= p; ++cnt; }
            fac_pr[n][l]  = (uint16_t)p;
            fac_exp[n][l] = (uint8_t)cnt;
            ++l;
        }
        fac_len[n] = (uint8_t)l;
    }

    /* factorisation of p-1 for primes p */
    for (int n = 2; n <= MAXN; ++n) {
        if (spf[n] != n) continue;            /* not prime */
        int x = n - 1, l = 0;
        while (x > 1) {
            int q = spf[x], cnt = 0;
            while (x % q == 0) { x /= q; ++cnt; }
            pf_pr[n][l]  = (uint16_t)q;
            pf_exp[n][l] = (uint8_t)cnt;
            ++l;
        }
        pf_len[n] = (uint8_t)l;
    }

    initialized = true;
}

/* --------------------------------------------------------------- */
/* add prime factors of a number to a list of distinct primes      */
/* --------------------------------------------------------------- */
static void add_factors(int num, int *primes, int *exps, int *sz)
{
    int l = fac_len[num];
    for (int t = 0; t < l; ++t) {
        int p = fac_pr[num][t];
        int e = fac_exp[num][t];
        int pos = -1;
        for (int u = 0; u < *sz; ++u) {
            if (primes[u] == p) { pos = u; break; }
        }
        if (pos >= 0) exps[pos] += e;
        else {
            primes[*sz] = p;
            exps[*sz]   = e;
            ++(*sz);
        }
    }
}

/* --------------------------------------------------------------- */
/* helper: add one factor (p-1) to the exponent list               */
/* --------------------------------------------------------------- */
static void add_p1_factors(int p, int *primes, int *exps, int *sz)
{
    int l = pf_len[p];
    for (int t = 0; t < l; ++t) {
        int q = pf_pr[p][t];
        int f = pf_exp[p][t];
        int pos = -1;
        for (int u = 0; u < *sz; ++u) {
            if (primes[u] == q) { pos = u; break; }
        }
        if (pos >= 0) exps[pos] += f;
        else {
            primes[*sz] = q;
            exps[*sz]   = f;
            ++(*sz);
        }
    }
}

/* --------------------------------------------------------------- */
/* main sequence function                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    if (n < 3) return 0;           /* no triplets for n = 1,2 */

    seq_t result = 0;
    int N = (int)n;                /* n ≤ MAXN */
    int max_i = N / 3;

    for (int i = 1; i <= max_i; ++i) {
        int max_j = (N - i) / 2;
        for (int j = i; j <= max_j; ++j) {
            int k = N - i - j;            /* k ≥ j automatically */

            /* collect prime exponents of i, j, k */
            int primes[32], exps[32];
            int sz = 0;
            add_factors(i, primes, exps, &sz);
            add_factors(j, primes, exps, &sz);
            add_factors(k, primes, exps, &sz);

            /* compute φ(i*j*k) exponents modulo 3 */
            int phi_pr[32], phi_mod[32];
            int phi_sz = 0;
            bool cube = true;

            for (int t = 0; t < sz; ++t) {
                int p = primes[t];
                int e_tot = exps[t];

                /* contribution from p^e_tot in φ: (e_tot-1) */
                int add = e_tot - 1;
                add %= 3; if (add < 0) add += 3;

                int pos = -1;
                for (int u = 0; u < phi_sz; ++u) {
                    if (phi_pr[u] == p) { pos = u; break; }
                }
                if (pos >= 0) phi_mod[pos] = (phi_mod[pos] + add) % 3;
                else {
                    phi_pr[phi_sz] = p;
                    phi_mod[phi_sz] = add;
                    ++phi_sz;
                }

                /* contribution from (p-1) factorisation */
                if (pf_len[p]) {
                    add_p1_factors(p, phi_pr, phi_mod, &phi_sz);
                }
            }

            /* check all exponents are multiples of 3 */
            for (int u = 0; u < phi_sz; ++u) {
                if (phi_mod[u] % 3 != 0) { cube = false; break; }
            }
            if (cube) ++result;
        }
    }

    return result;
}
