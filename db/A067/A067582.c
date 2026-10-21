/*
 * A067582  Nonprime values of n such that
 *          bigomega(n)^omega(n)=omega(n)^bigomega(n) where bigomega(n) is the
 *          number of prime factors of n (counted with multiplicity) and
 *          omega(n) the number of distinct prime factors of n.
 * https://oeis.org/A067582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Nonprime numbers n such that Ω(n)^ω(n)=ω(n)^Ω(n). Equivalently,
 *            n=1 or n is a square‑free composite number or n=p*q³ or n=p²*q²
 *            for distinct primes p and q. The sequence is listed in
 *            increasing order
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LIMIT 200000   /* enough for n up to ~12e4 terms, far beyond the 10k limit */

static bool initialized = false;
static uint32_t *spf = NULL;          /* smallest prime factor array */
static uint32_t *prime_list = NULL;   /* list of primes for sieve */
static size_t prime_cnt = 0;

static seq_t *vals = NULL;            /* array of sequence values */
static size_t vals_sz = 0;
static size_t vals_cap = 0;

/* Linear sieve to fill spf up to limit */
static void compute_spf(void)
{
    uint32_t *s = (uint32_t *)calloc(MAX_LIMIT + 1, sizeof(uint32_t));
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * (MAX_LIMIT / 10 + 10));
    size_t pc = 0;
    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (s[i] == 0) {
            s[i] = i;
            pr[pc++] = i;
        }
        for (size_t j = 0; j < pc; ++j) {
            uint32_t p = pr[j];
            if (p > s[i] || (uint64_t)i * p > MAX_LIMIT)
                break;
            s[i * p] = p;
        }
    }
    spf = s;
    prime_list = pr;
    prime_cnt = pc;
}

/* Helper: check whether n satisfies the condition */
static bool good_n(uint32_t n)
{
    if (n == 1)
        return true;
    uint32_t m = n;
    size_t distinct = 0;
    size_t total = 0;
    bool squarefree = true;
    while (m > 1) {
        uint32_t p = spf[m];
        size_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if (e > 1)
            squarefree = false;
        ++distinct;
        total += e;
        if (total > 4)      /* early exit */
            return false;
    }
    if (squarefree && distinct > 1)
        return true;
    if (distinct == 2 && total == 4)
        return true;
    return false;
}

/* Build the sequence values up to MAX_LIMIT */
static void init_seq(void)
{
    if (initialized)
        return;
    compute_spf();

    /* Allocate space for values; will grow if needed */
    vals_cap = MAX_LIMIT;
    vals = (seq_t *)malloc(vals_cap * sizeof(seq_t));

    for (uint32_t i = 1; i <= MAX_LIMIT; ++i) {
        if (good_n(i)) {
            if (vals_sz >= vals_cap) {
                vals_cap *= 2;
                vals = (seq_t *)realloc(vals, vals_cap * sizeof(seq_t));
            }
            vals[vals_sz++] = (seq_t)i;
        }
    }
    initialized = true;
    /* We could free spf and primes after use, but keeping them is cheap. */
}

/* Sequence function required by the build */
seq_t a(idx_t n)
{
    init_seq();                          /* ensure data are ready */
    if ((size_t)n == 0)
        return 0;                        /* out of range, but never called with n=0 */
    if ((size_t)n > vals_sz)
        return 0;                        /* safety fall‑back */
    return vals[(size_t)n - 1];
}
