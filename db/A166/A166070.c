/*
 * A166070  Sorted sequence of primes and multiply perfect numbers.
 * https://oeis.org/A166070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 76 terms through n = 865 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all positive integers up to a fixed limit, test whether each
   is a prime or a multiply‑perfect number (including 1), and store the results
   in a sorted list. a(n) returns the n-th element of this list. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool is_multiply_perfect(idx_t k, const uint32_t *spf)
{
    if (k < 2) return false;  /* 1 is handled separately */

    /* factorisation of k (at most 7 distinct primes for k <= 200000) */
    idx_t t = k;
    uint32_t primes[8];
    int    exps[8];
    int    pcnt = 0;
    while (t > 1) {
        uint32_t p = spf[t];
        int e = 0;
        while (t % p == 0) {
            t /= p;
            ++e;
        }
        primes[pcnt]   = p;
        exps[pcnt]     = e;
        ++pcnt;
    }

    /* compute sigma(k) = Π (p^(e+1)-1)/(p-1) */
    seq_t sigma = 1;
    for (int i = 0; i < pcnt; ++i) {
        seq_t pow = 1;
        for (int e = 0; e <= exps[i]; ++e) {
            pow *= (seq_t)primes[i];
        }
        seq_t denom = (seq_t)primes[i] - 1;
        seq_t geom  = (pow - 1) / denom;
        sigma *= geom;
    }

    /* enumerate all proper divisors of k */
    seq_t divs[2048];
    size_t sz = 1;
    divs[0] = 1;
    for (int i = 0; i < pcnt; ++i) {
        size_t cur = sz;
        seq_t mul = 1;
        for (int e = 1; e <= exps[i]; ++e) {
            mul *= (seq_t)primes[i];
            for (size_t j = 0; j < cur; ++j) {
                divs[sz++] = divs[j] * mul;
            }
        }
    }

    /* check the multiply‑perfect condition */
    for (size_t i = 0; i < sz; ++i) {
        seq_t d = divs[i];
        if (d <= 1 || d == k) continue;
        if (sigma % d != 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* -------------------------------------------------------------
       static state for lazy one‑time initialisation
       ------------------------------------------------------------- */
    static bool      initialized = false;
    static seq_t    *terms       = NULL;
    static size_t    terms_cnt   = 0;
    static size_t    terms_cap   = 0;
    static uint32_t *spf         = NULL;   /* smallest prime factor table */

    if (!initialized) {
        const idx_t LIMIT = 200000;   /* enough for n ≤ 10 000 */
        /* ---------- 1. build SPF sieve up to LIMIT ---------- */
        spf = (uint32_t *)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
        if (!spf) abort();   /* out of memory */

        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {          /* prime */
                spf[i] = (uint32_t)i;
                if (i <= LIMIT / i) {
                    for (idx_t j = i * i; j <= LIMIT; j += i) {
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                    }
                }
            }
        }

        /* ---------- 2. allocate space for the sequence ---------- */
        terms_cap = 5000;
        terms     = mem_new(seq_t, terms_cap);
        if (!terms) abort();

        /* ---------- 3. compute all terms up to LIMIT ---------- */
        for (idx_t k = 1; k <= LIMIT; ++k) {
            bool add = false;
            if (k == 1) add = true;
            else if (spf[k] == k) add = true;          /* prime */
            else if (is_multiply_perfect(k, spf)) add = true;

            if (add) {
                if (terms_cnt >= terms_cap) {
                    terms_cap <<= 1;
                    terms = mem_resize(seq_t, terms, terms_cap);
                    if (!terms) abort();
                }
                terms[terms_cnt++] = (seq_t)k;
            }
        }
        /* ---------- 4. free SPF table ---------- */
        free(spf);
        spf = NULL;

        initialized = true;
    }

    if (n == 0) return 0;         /* undefined for n<1, return 0 safely */
    if ((idx_t)(n - 1) < terms_cnt) return terms[n - 1];
    return 0;                     /* should never happen for n ≤ 10 000 */
}
