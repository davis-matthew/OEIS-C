/*
 * A075394  Numbers n such that S(n-1) < S(n) > S(n+1), where S(n) is the
 *          Kempner function A002034(n).
 * https://oeis.org/A075394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   min_m_for_pe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that S(n‑1) < S(n) > S(n+1),
 * where S is the Kempner function A002034. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/min_m_for_pe.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static bool init = false;
    static size_t limit = 0;               /* current maximum examined n       */
    static size_t terms_cnt = 0;           /* number of terms stored           */
    static seq_t *terms = NULL;            /* array of terms (a(1),a(2),…)    */
    static size_t terms_cap = 0;
    static uint32_t *spf = NULL;           /* smallest prime factor table      */
    static size_t spf_cap = 0;
    static seq_t *S = NULL;                /* Kempner function values           */
    static size_t S_cap = 0;

    if (!init) {
        init = true;
        limit = 1;
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_cnt = 0;
    }

    /* acquire enough terms for requested index n */
    while (terms_cnt < n) {
        /* increase search limit */
        size_t new_limit = limit * 2 + 100;
        if (new_limit < 100) new_limit = 100;

        /* allocate / resize spf array */
        if (spf_cap < new_limit + 2) {
            spf_cap = new_limit + 2;
            uint32_t *tmp = (uint32_t *)realloc(spf, spf_cap * sizeof(uint32_t));
            spf = tmp;
        }
        /* zero‑out spf to build fresh */
        for (size_t i = 0; i <= new_limit + 1; ++i) spf[i] = 0;

        /* simple sieve to fill spf */
        for (size_t i = 2; i <= new_limit + 1; ++i) {
            if (spf[i] == 0) {
                spf[i] = (uint32_t)i;
                for (size_t j = i; j <= new_limit + 1; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }

        /* allocate / resize S array */
        if (S_cap < new_limit + 2) {
            S_cap = new_limit + 2;
            seq_t *tmp = (seq_t *)realloc(S, S_cap * sizeof(seq_t));
            S = tmp;
        }
        S[0] = (seq_t)0;      /* by convention S(0)=0 */
        S[1] = (seq_t)0;      /* Kempner function of 1 is 0 */

        /* compute S(i) for i=2..new_limit+1 */
        for (size_t i = 2; i <= new_limit + 1; ++i) {
            uint64_t x = i;
            seq_t best = (seq_t)0;
            while (x > 1) {
                uint32_t p = spf[x];
                int cnt = 0;
                while (x % p == 0) {
                    x /= p;
                    ++cnt;
                }
                seq_t m = min_m_for_pe(p, (uint64_t)cnt);
                if (m > best) best = m;
            }
            S[i] = best;
        }

        /* rebuild term list for i=2..new_limit */
        if (terms) free(terms);
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_cnt = 0;
        for (size_t i = 2; i <= new_limit; ++i) {
            if (S[i - 1] < S[i] && S[i] > S[i + 1]) {
                if (terms_cnt == terms_cap) {
                    terms_cap <<= 1;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_cnt++] = (seq_t)i;
            }
        }

        limit = new_limit;
    }

    return terms[n - 1];
}
