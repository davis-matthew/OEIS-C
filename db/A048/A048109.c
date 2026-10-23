/*
 * A048109  Numbers having equally many squarefree and nonsquarefree divisors;
 *          number of unitary divisors of n (A034444) = number of non-unitary
 *          divisors of n (A048105).
 * https://oeis.org/A048109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4865, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048109: numbers n such that d(n)=2^(ω(n)+1) */
/* (d = divisor count, ω = distinct prime factor count) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* ----- static cache & sieve state ----- */
    static uint32_t *spf = NULL;      /* smallest prime factor table */
    static size_t   spf_len = 0;      /* current sieve limit  */
    static seq_t   *terms = NULL;     /* found terms of the sequence */
    static size_t   term_cnt = 0;     /* number of terms stored */
    static size_t   term_cap = 0;     /* capacity of terms array   */
    static idx_t    last = 1;         /* last integer tested       */
    static bool     initialized = false;

    /* ---------------- initialization ---------------- */
    if (!initialized) {
        /* start with a modest sieve size; will grow if needed */
        const size_t INIT_LIMIT = 2000000;
        spf = (uint32_t *)calloc(INIT_LIMIT + 1, sizeof(uint32_t));
        if (!spf) return 0;          /* allocation failure sentinel */

        /* linear sieve to fill spf */
        uint32_t *prime_list = (uint32_t *)malloc((INIT_LIMIT + 1) * sizeof(uint32_t));
        if (!prime_list) { free(spf); return 0; }

        size_t prime_index = 0;
        for (size_t i = 2; i <= INIT_LIMIT; ++i) {
            if (spf[i] == 0) {
                spf[i] = (uint32_t)i;
                prime_list[prime_index++] = (uint32_t)i;
            }
            for (size_t j = 0; j < prime_index &&
                 prime_list[j] <= spf[i] &&
                 i * prime_list[j] <= INIT_LIMIT; ++j) {
                spf[i * prime_list[j]] = prime_list[j];
            }
        }
        free(prime_list);
        spf_len = INIT_LIMIT;
        initialized = true;
    }

    /* ----------- extend sieve if needed -------------- */
    if (last > (idx_t)spf_len) {
        const size_t NEW_LIMIT = spf_len * 2;
        uint32_t *new_spf = (uint32_t *)calloc(NEW_LIMIT + 1, sizeof(uint32_t));
        if (!new_spf) return 0;

        uint32_t *prime_list = (uint32_t *)malloc((NEW_LIMIT + 1) * sizeof(uint32_t));
        if (!prime_list) { free(new_spf); return 0; }

        size_t prime_index = 0;
        for (size_t i = 2; i <= NEW_LIMIT; ++i) {
            if (new_spf[i] == 0) {
                new_spf[i] = (uint32_t)i;
                prime_list[prime_index++] = (uint32_t)i;
            }
            for (size_t j = 0; j < prime_index &&
                 prime_list[j] <= new_spf[i] &&
                 i * prime_list[j] <= NEW_LIMIT; ++j) {
                new_spf[i * prime_list[j]] = prime_list[j];
            }
        }
        free(prime_list);
        free(spf);
        spf = new_spf;
        spf_len = NEW_LIMIT;
    }

    /* ---------------- generate terms ---------------- */
    while (term_cnt < n) {
        ++last;                       /* test next integer */

        /* factor last using spf */
        idx_t  x = last;
        seq_t divisor_count = 1;
        idx_t omega = 0;               /* distinct primes */

        while (x > 1) {
            uint32_t p = spf[(size_t)x];
            size_t exp = 0;
            do {
                x /= p;
                ++exp;
            } while (x % p == 0);
            divisor_count *= (seq_t)(exp + 1);
            ++omega;
        }

        /* check the defining condition: d(n) = 2^(ω+1) */
        if (divisor_count == pow2(omega + 1)) {
            /* store the term */
            if (term_cnt >= term_cap) {
                term_cap = (term_cap == 0) ? 16 : term_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                terms = tmp;
            }
            terms[term_cnt++] = (seq_t)last;
        }
    }

    /* -------- return the requested term (1‑based) -------- */
    return terms[n - 1];
}
