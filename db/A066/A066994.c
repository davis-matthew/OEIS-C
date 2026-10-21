/*
 * A066994  Numbers k such that phi(k) divides k*sigma(k).
 * https://oeis.org/A066994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 2409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth integer k such that φ(k) divides k·σ(k)  */
/* φ(k) – Euler's totient, σ(k) – sum of divisors.  */
/* The function returns the nth term of OEIS sequence A066994.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_K 2000000  /* upper bound for searching */

static uint32_t *spf = NULL;          /* smallest prime factor array */
static size_t spf_size = 0;           /* current size of spf */
static seq_t *terms = NULL;           /* array of found terms */
static size_t terms_cap = 0;           /* allocated capacity */
static size_t terms_len = 0;           /* number of terms stored */
static uint64_t next_k = 0;           /* next integer to test */

/* build smallest‑prime‑factor table up to limit */
static void build_spf(void)
{
    if (spf)
        return;
    spf_size = MAX_K + 1;
    spf = (uint32_t *)malloc(spf_size * sizeof(uint32_t));
    for (uint32_t i = 0; i < spf_size; ++i)
        spf[i] = i;
    spf[0] = 0;
    spf[1] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= MAX_K; ++i) {
        if (spf[i] == i) {            /* i is prime */
            for (uint32_t j = i * i; j <= MAX_K; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }
}

/* test whether k satisfies φ(k) | k·σ(k) */
static bool good(uint64_t k)
{
    if (k == 1) return true;
    uint64_t temp = k;
    uint64_t phi = 1;
    uint64_t sigma = 1;
    while (temp > 1) {
        uint32_t p = spf[temp];
        uint64_t p_pow = 1;
        while (temp % p == 0) {
            temp /= p;
            p_pow *= p;
        }
        /* φ part: φ(p^e) = p^(e-1)·(p-1) */
        phi *= (p_pow / p) * (p - 1);
        /* σ part: σ(p^e) = (p^(e+1)-1)/(p-1) */
        uint64_t num = p_pow * (uint64_t)p - 1;
        sigma *= num / (p - 1);
    }
    __int128 prod = (__int128)k * sigma;
    return (prod % (__int128)phi) == 0;
}

/* append a term to the terms array */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = new_terms;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n<1 */

    build_spf();

    /* initialize generation if first call */
    if (terms_len == 0) {
        next_k = 1;
    }

    /* generate terms until we have n */
    while (terms_len < n) {
        if (next_k >= MAX_K) {
            /* safety: should not happen for n ≤ 10000 */
            /* extend spf if needed */
            size_t new_limit = spf_size * 2;
            uint32_t *new_spf = (uint32_t *)realloc(spf,
                            new_limit * sizeof(uint32_t));
            for (uint32_t i = spf_size; i < new_limit; ++i)
                new_spf[i] = i;
            for (uint32_t i = 2; (uint64_t)i * i < new_limit; ++i) {
                if (new_spf[i] == i) {
                    for (uint32_t j = i * i; j < new_limit; j += i)
                        if (new_spf[j] == j)
                            new_spf[j] = i;
                }
            }
            spf = new_spf;
            spf_size = new_limit;
        }

        if (good(next_k))
            append_term((seq_t)next_k);
        ++next_k;
    }

    return terms[n - 1];
}
