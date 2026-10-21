/*
 * A067207  Numbers k such that the digits of sigma_2(k) end in k.
 * https://oeis.org/A067207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more, hard
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A067207: numbers k such that σ₂(k) ends with k.
 * The function returns the nth term by generating terms on demand
 * with a small SPF sieve and prime‑factorial σ₂ computation.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* --- Smallest‑prime‑factor sieve ------------------------------------- */
static uint32_t *spf = NULL;          /* spf[i] = smallest prime factor of i   */
static size_t    spf_limit = 0;       /* highest number for which spf is valid */

/* Build spf up to 'limit'.  Memory: ~4*limit bytes. */
static void build_spf(size_t limit)
{
    free(spf);
    spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!spf) return;                /* out of memory – caller will fail */

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
                }
            }
        }
    }
    spf_limit = limit;
}

/* Extend the spf table if necessary. */
static void ensure_spf_limit(size_t limit)
{
    if (limit <= spf_limit) return;
    size_t new_limit = spf_limit ? spf_limit * 2 : 1000000;
    while (new_limit < limit) new_limit *= 2;
    build_spf(new_limit);
}

/* --- σ₂(n) via prime factorisation --------------------------------- */
static seq_t sigma2_uint64(uint64_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t tmp   = n;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        if (p == 0) p = (uint32_t)tmp;   /* in case tmp exceeds current spf */
        unsigned exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }
        /* σ₂(p^exp) = (p^{2(exp+1)} – 1) / (p² – 1) */
        seq_t p2 = (seq_t)p * (seq_t)p;   /* p² */
        seq_t p2exp = (seq_t)1;
        for (unsigned i = 0; i < exp + 1; ++i) p2exp *= p2; /* (p²)^{exp+1} */
        seq_t contrib = (p2exp - (seq_t)1) / (p2 - (seq_t)1);
        result *= contrib;
    }
    return result;
}

/* --- Sequence storage ----------------------------------------------- */
static seq_t  *terms = NULL;      /* found terms in order */
static size_t  terms_len = 0;     /* number of stored terms */
static size_t  terms_cap = 0;     /* allocated capacity */
static uint64_t next_candidate = 1;  /* next number to test */

/* --- public function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise once */
    static bool initialized = false;
    if (!initialized) {
        /* Start with a moderate spf size; will grow if necessary. */
        build_spf(20000000);           /* 20 000 000 ≈ 80 MB for uint32_t array */
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {

        /* Grow the spf table if the candidate exceeds current limit */
        if ((size_t)next_candidate > spf_limit) {
            ensure_spf_limit((size_t)next_candidate * 2);
        }

        /* Compute σ₂(next_candidate) */
        seq_t sigma = sigma2_uint64((uint64_t)next_candidate);

        /* Last‑digit test: σ₂ modulo 10^d == next_candidate */
        idx_t d = digit_len((seq_t)next_candidate);  /* number of decimal digits */
        seq_t mod = pow10(d);                       /* 10^d  */
        if (sigma % mod == (seq_t)next_candidate) {
            /* Store this term */
            if (terms_len >= terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
