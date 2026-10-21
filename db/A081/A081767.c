/*
 * A081767  Numbers k such that k^2 - 1 divides binomial(2k,k).
 * https://oeis.org/A081767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 7452, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A081767: Numbers k such that k^2-1 | C(2k,k) */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* -- helper: smallest prime factor sieve --------------------------------- */
static uint32_t *spf = NULL;     /* spf[x] = smallest prime factor of x  */
static size_t spf_max = 0;       /* maximum index computed so far       */

static void init_spf(size_t n)
{
    size_t i, j;
    spf = (uint32_t*)realloc(spf, (n + 1) * sizeof(uint32_t));
    for (i = 0; i <= n; ++i) spf[i] = (uint32_t)i;
    for (i = 2; i * i <= n; ++i)
        if (spf[i] == (uint32_t)i)
            for (j = i * i; j <= n; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
    spf_max = n;
}

static inline void ensure_spf(size_t n)
{
    if (n <= spf_max) return;
    size_t newmax = (n < 2 * spf_max) ? 2 * spf_max : n;
    init_spf(newmax);
}

/* -- helper: exponent of prime p in m! (Legendre) ------------------------ */
static uint64_t legendre(uint64_t m, uint64_t p)
{
    uint64_t e = 0;
    while (m) { m /= p; e += m; }
    return e;
}

/* -- accumulate prime powers of a value v into arrays --------------------- */
static void factor_accumulate(uint64_t v,
                              uint32_t *pr, uint8_t *ex, int *cnt)
{
    while (v > 1) {
        uint32_t p = spf[v];
        uint8_t e = 0;
        while (v % p == 0) { v /= p; ++e; }
        /* look for p in the array */
        int i;
        for (i = 0; i < *cnt; ++i)
            if (pr[i] == p) break;
        if (i == *cnt) {          /* new prime */
            pr[i] = p; ex[i] = e; ++*cnt;
        } else {
            ex[i] += e;
        }
    }
}

/* -- test whether k satisfies the condition ------------------------------ */
static int passes(uint64_t k)
{
    /* collect prime powers of (k-1)(k+1) */
    uint32_t primes[32];
    uint8_t exps[32];
    int pcnt = 0;

    factor_accumulate(k-1, primes, exps, &pcnt);
    factor_accumulate(k+1, primes, exps, &pcnt);

    /* now check against C(2k,k) */
    for (int i = 0; i < pcnt; ++i) {
        uint32_t p = primes[i];
        uint8_t need = exps[i];
        uint64_t have = legendre(2*k, p) - 2*legendre(k, p);
        if (have < need) return 0;
    }
    return 1;
}

/* -- cache of found terms ------------------------------------------------- */
static seq_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;
static uint64_t next_candidate = 2;   /* smallest k we have not yet tested */

static void push_term(uint64_t v)
{
    if (term_cap == term_len) {
        size_t newcap = (term_cap == 0) ? 32 : term_cap << 1;
        terms = (seq_t*)realloc(terms, newcap * sizeof(seq_t));
        term_cap = newcap;
    }
    terms[term_len++] = (seq_t)v;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe */

    /* keep generating terms until we have at least n of them */
    while (term_len < (size_t)n) {
        ensure_spf(next_candidate + 1);   /* sieve up to k+1 */
        if (passes(next_candidate)) push_term(next_candidate);
        ++next_candidate;
    }
    return terms[(size_t)n - 1];
}
