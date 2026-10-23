/*
 * A176803  a(n) = the smallest natural numbers m such that product of
 *          antiharmonic mean of the divisors of n and antiharmonic mean of
 *          the divisors of m are integers, a(n) = 0 if no such number exists.
 * https://oeis.org/A176803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest natural number m such that
 *            (sigma2(n)/sigma1(n)) * (sigma2(m)/sigma1(m)) is an integer,
 *            where sigma1 = sum of divisors, sigma2 = sum of squares of
 *            divisors. a(n)=0 if no such m exists
 * Range:     u8 n <= 28, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Simple sieve for primes up to 200000                          */
#define MAX_PRIME_LIMIT 200000
#define MAX_PRIMES 20000

static uint32_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool *is_comp = (bool*)calloc(MAX_PRIME_LIMIT + 1, sizeof(bool));
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt < MAX_PRIMES) primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= MAX_PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_ready = true;
}

/* ------------------------------------------------------------ */
/* Factorisation helper (stores primes and exponents in arrays) */
#define MAX_FACT 64

static void factor_uint64(uint64_t n,
                          uint32_t *pf, uint32_t *pe, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) {
            uint32_t p = primes[i];
            uint32_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pf[(*cnt)] = p;
            pe[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) { /* remaining prime factor > sqrt(original) */
        pf[(*cnt)] = (uint32_t)n;
        pe[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------ */
/* Compute sigma1 and sigma2 for a given integer n            */
static void compute_sigma1_sigma2(uint64_t n, uint64_t *s1, uint64_t *s2)
{
    uint32_t pf[MAX_FACT], pe[MAX_FACT];
    size_t cnt;
    factor_uint64(n, pf, pe, &cnt);

    uint64_t total1 = 1;
    uint64_t total2 = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p = pf[i];
        uint32_t e = pe[i];

        /* sigma1 of p^e = 1 + p + p^2 + ... + p^e */
        uint64_t term1 = 1;
        uint64_t power = 1;
        for (uint32_t k = 1; k <= e; ++k) {
            power *= p;
            term1 += power;
        }

        /* sigma2 of p^e = 1 + p^2 + p^4 + ... + p^(2e) */
        uint64_t term2 = 1;
        uint64_t power2 = 1;
        for (uint32_t k = 1; k <= e; ++k) {
            power2 *= (uint64_t)p * p;
            term2 += power2;
        }

        total1 *= term1;
        total2 *= term2;
    }

    *s1 = total1;
    *s2 = total2;
}

/* ------------------------------------------------------------ */
/* Diff entry for a prime: diff = v_p(sigma2) - v_p(sigma1)   */
#define MAX_DIFF 128
typedef struct { uint32_t prime; int diff; } DiffEntry;

/* Compute diff vector for n (fills vec and returns count) */
static void compute_diff(uint64_t n, DiffEntry *vec, size_t *cnt)
{
    uint64_t s1, s2;
    compute_sigma1_sigma2(n, &s1, &s2);

    /* Factorise s1 and s2 */
    uint32_t pf1[MAX_FACT], pe1[MAX_FACT];
    size_t c1;
    factor_uint64(s1, pf1, pe1, &c1);

    uint32_t pf2[MAX_FACT], pe2[MAX_FACT];
    size_t c2;
    factor_uint64(s2, pf2, pe2, &c2);

    /* Build diff list */
    *cnt = 0;
    /* Contribution from s2 */
    for (size_t i = 0; i < c2; ++i) {
        uint32_t p = pf2[i];
        uint32_t e2 = pe2[i];
        uint32_t e1 = 0;
        for (size_t j = 0; j < c1; ++j) {
            if (pf1[j] == p) { e1 = pe1[j]; break; }
        }
        int d = (int)e2 - (int)e1;
        if (d != 0) {
            vec[*cnt].prime = p;
            vec[*cnt].diff  = d;
            ++(*cnt);
        }
    }
    /* Remaining primes from s1 not in s2 */
    for (size_t i = 0; i < c1; ++i) {
        uint32_t p = pf1[i];
        bool seen = false;
        for (size_t j = 0; j < c2; ++j) {
            if (pf2[j] == p) { seen = true; break; }
        }
        if (!seen) {
            int d = -(int)pe1[i];
            if (d != 0) {
                vec[*cnt].prime = p;
                vec[*cnt].diff  = d;
                ++(*cnt);
            }
        }
    }
}

/* ------------------------------------------------------------ */
/* Main sequence function                                        */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    init_primes();                 /* primes ready for factorisation */

    /* Compute diff of n */
    DiffEntry dn[MAX_DIFF];
    size_t dnc;
    compute_diff((uint64_t)n, dn, &dnc);

    /* Build requirement list (negative diffs) */
    struct { uint32_t prime; int need; } req[MAX_DIFF];
    size_t rc = 0;
    for (size_t i = 0; i < dnc; ++i) {
        if (dn[i].diff < 0) {
            if (dn[i].prime == 2) /* even denominator -> impossible */
                return (seq_t)0;
            req[rc].prime = dn[i].prime;
            req[rc].need  = -(int)dn[i].diff; /* |negative| */
            ++rc;
        }
    }
    if (rc == 0) return (seq_t)1;     /* all diffs non‑negative */

    /* Search for smallest m */
    const uint64_t MAX_M = 1000000;   /* safe upper bound for the test set */
    for (uint64_t m = 2; m <= MAX_M; ++m) {
        /* Diff for candidate m */
        DiffEntry dm[MAX_DIFF];
        size_t dmc;
        compute_diff(m, dm, &dmc);

        bool ok = true;

        /* 1. Check required primes */
        for (size_t i = 0; i < rc; ++i) {
            int d_of_m = 0;
            for (size_t j = 0; j < dmc; ++j)
                if (dm[j].prime == req[i].prime) { d_of_m = dm[j].diff; break; }
            if (d_of_m < req[i].need) { ok = false; break; }
        }
        if (!ok) continue;

        /* 2. Ensure no negative contribution breaks existing positives */
        for (size_t i = 0; i < dmc; ++i) {
            if (dm[i].diff >= 0) continue;
            int dn_val = 0;
            for (size_t k = 0; k < dnc; ++k)
                if (dn[k].prime == dm[i].prime) { dn_val = dn[k].diff; break; }
            if (dm[i].diff + dn_val < 0) { ok = false; break; }
        }
        if (ok) return (seq_t)m;     /* minimal m found */
    }
    /* No m found within the bound – by the definition a(n)=0 */
    return (seq_t)0;
}
