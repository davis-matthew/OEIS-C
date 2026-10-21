/*
 * A060665  Numbers k such that sigma(x) = k has exactly 9 solutions.
 * https://oeis.org/A060665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 119, u32 n <= 9798, u64 n <= 9798,
 *            u128 n <= 9798, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computing OEIS A060665 – numbers k for which σ(n) = k has exactly
    9 solutions.  This routine does a one‑time sieve up to a fixed
    maximum and then serves up the requested term from a static array.
    The sieve computes σ(n) for all n ≤ KMAX, counts the occurrences
    of each σ value, and records those with a count of precisely 9.
    Because for any such σ value k all solutions n satisfy n < k,
    taking KMAX sufficiently large (here 20 000 000) captures all
    required terms for n up to about ten thousand. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of bounds */

    static seq_t *terms = NULL;      /* array of found terms          */
    static size_t terms_len = 0;     /* number of terms stored        */
    static size_t terms_cap = 0;     /* allocated capacity of terms    */
    static int initialized = 0;      /* whether precomputation done    */

    if (!initialized) {
        /* -------------------------------------------------- */
        /* 1. Precompute σ(n) for 1 ≤ n ≤ KMAX                 */
        /* -------------------------------------------------- */
        const idx_t KMAX = 20000000;            /* 20 million is ample          */
        const size_t SZ  = (size_t)(KMAX + 1);

        /* sigma[i] holds σ(i).  We start with σ(i)=1 for all i>0
           to account for the divisor 1; the value for σ(1) stays
           as 1.  σ(0) is unused and set to 0.                */
        uint32_t *sigma = (uint32_t *)malloc(SZ * sizeof(uint32_t));
        if (!sigma) return 0;                   /* allocation failed            */
        for (size_t i = 0; i < SZ; ++i) sigma[i] = 1;
        sigma[0] = 0;

        for (uint32_t i = 2; i <= KMAX; ++i) {
            for (uint32_t j = i; j <= KMAX; j += i) {
                sigma[j] += i;
            }
        }

        /* -------------------------------------------------- */
        /* 2. Count occurrences of each σ value              */
        /* -------------------------------------------------- */
        /* uint16_t is used to avoid overflow for counts.
           For most σ values the count is far below 256.      */
        uint16_t *cnt = (uint16_t *)calloc(SZ, sizeof(uint16_t));
        if (!cnt) {
            free(sigma);
            return 0;
        }

        for (uint32_t i = 1; i <= KMAX; ++i) {
            uint32_t s = sigma[i];
            if (s <= KMAX) {
                /* Increment count; it won't overflow 16 bits.  */
                cnt[s] += 1;
            }
        }

        /* -------------------------------------------------- */
        /* 3. Record all k with exactly 9 solutions           */
        /* -------------------------------------------------- */
        for (uint32_t v = 1; v <= KMAX; ++v) {
            if (cnt[v] == 9) {
                if (terms_len >= terms_cap) {
                    terms_cap = terms_cap ? terms_cap * 2 : 128;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) {
                        free(sigma);
                        free(cnt);
                        return 0;
                    }
                    terms = tmp;
                }
                terms[terms_len++] = (seq_t)v;
            }
        }

        /* Clean up */
        free(sigma);
        free(cnt);
        initialized = 1;
    }

    /* Return the requested term or 0 if out of range. */
    if (n > terms_len) return 0;
    return terms[n - 1];
}
