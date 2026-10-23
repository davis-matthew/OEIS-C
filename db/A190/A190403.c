/*
 * A190403  Number n for which sigma(n)=sigma(n'), where sigma is the sum of
 *          divisors and n' the arithmetic derivative of n.
 * https://oeis.org/A190403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1132, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A190403: Numbers n for which σ(n)=σ(n'), where n' is the arithmetic derivative of n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"

/* Local helper: compute σ from prime factorisation */
static seq_t
sigma_from_factors(const uint64_t *pr, const uint64_t *ex, size_t cnt)
{
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term  = 1;          /* 1 + p + p² + ... + pᵉ */
        seq_t pow   = 1;          /* current pᵏ   */
        for (uint64_t k = 0; k < ex[i]; ++k) {
            pow  = pow * (seq_t)pr[i];
            term = term + pow;
        }
        res = res * term;
    }
    return res;
}

/* Dynamic array of terms */
static seq_t *terms   = NULL;
static size_t terms_count = 0;
static size_t terms_cap   = 0;

/* Ensure that at least `target` terms are available */
static void
ensure_terms(idx_t target)
{
    if (terms == NULL) {
        terms_cap = 1024;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();
    }

    /* Candidate starts from the first natural number (2) if no terms yet,
       otherwise just after the last found term. */
    uint64_t cand = 2;
    if (terms_count > 0)
        cand = (uint64_t)terms[terms_count - 1] + 1;

    /* Temporary storage for factorisation */
    static uint64_t pr[64], ex[64];
    static uint64_t pr_d[64], ex_d[64];
    size_t cnt, cnt_d;

    while (terms_count < (size_t)target) {
        /* factorise cand */
        factor_u64(cand, pr, ex, &cnt);

        /* σ(cand) */
        seq_t sigma_cand = sigma_from_factors(pr, ex, cnt);

        /* arithmetic derivative: Σ (cand / p) * e  */
        uint64_t deriv = 0;
        for (size_t i = 0; i < cnt; ++i)
            deriv += (cand / pr[i]) * ex[i];

        if (deriv != 0) {
            /* factorise derivative */
            factor_u64(deriv, pr_d, ex_d, &cnt_d);
            seq_t sigma_der = sigma_from_factors(pr_d, ex_d, cnt_d);

            if (sigma_cand == sigma_der) {
                if (terms_count == terms_cap) {
                    terms_cap *= 2;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!terms) abort();
                }
                terms[terms_count++] = (seq_t)cand;
            }
        }
        ++cand;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
