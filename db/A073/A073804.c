/*
 * A073804  Numbers k such that the number of divisors of k is greater than
 *          that of sigma(k).
 * https://oeis.org/A073804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that d(k) > d(σ(k)) where d(x) is
 *            the number of positive divisors of x and σ(x) is the sum of
 *            positive divisors of x
 * Range:     u8 n <= 24, u16 n <= 1069, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_uint64.h"

/*  Static storage for memoisation  */
static seq_t *terms = NULL;         /* array of found terms */
static idx_t  terms_alloc = 0;      /* allocated capacity of terms  */
static idx_t  terms_len   = 0;      /* number of terms stored       */
static uint64_t checked   = 0;      /* last number that was tested  */

/*  Ensure that at least n needed terms are stored  */
static void
ensure_terms(idx_t n_needed)
{
    if (terms_len >= n_needed) return;

    if (!terms) {
        terms_alloc = 256;
        terms = (seq_t *)malloc(terms_alloc * sizeof(seq_t));
    }

    uint64_t fac[20];
    int exp[20];
    int cnt;

    /*  Test successive k until we have n_needed terms  */
    while (terms_len < n_needed) {
        ++checked;                    /* candidate k = checked */
        uint64_t k = checked;

        /* factor k */
        factor_uint64(k, fac, exp, &cnt);

        /* compute τ(k) and σ(k) */
        uint64_t tau_k = 1;
        __int128 sigma = 1;

        for (int i = 0; i < cnt; ++i) {
            uint64_t e = (uint64_t)exp[i];
            tau_k *= (e + 1);

            /* sum of divisors for p^e : (p^{e+1}-1)/(p-1)  */
            __int128 p_pow = 1;
            for (uint64_t j = 0; j < e; ++j) p_pow *= fac[i];
            __int128 sum_part = (p_pow * fac[i] - 1) / (fac[i] - 1);
            sigma *= sum_part;
        }

        /* factor σ(k) to compute τ(σ(k)) */
        uint64_t sigma_val = (uint64_t)sigma;   /* σ(k) fits in 64‑bit for needed range */
        uint64_t fac2[20];
        int exp2[20];
        int cnt2;
        factor_uint64(sigma_val, fac2, exp2, &cnt2);

        uint64_t tau_sigma = 1;
        for (int j = 0; j < cnt2; ++j) {
            tau_sigma *= (uint64_t)(exp2[j] + 1);
        }

        if (tau_k > tau_sigma) {
            if (terms_len >= terms_alloc) {
                terms_alloc <<= 1;
                terms = (seq_t *)realloc(terms, terms_alloc * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)k;
        }
    }
}

/*  Public API – returns the nth element (1‑based) of the sequence  */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return terms[n - 1];
}
