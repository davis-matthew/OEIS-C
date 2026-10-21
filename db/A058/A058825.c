/*
 * A058825  Numbers which are both totients and cototients.
 * https://oeis.org/A058825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by intersection of totients and cototients (search)
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The function returns the nth term of OEIS sequence A058825. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined for n=0 */

    /* static state to allow memoisation across calls */
    static seq_t *vec = NULL;
    static size_t vec_len = 0;                   /* number of terms stored */
    static uint64_t L = 200000;                  /* current limit for φ and cototients */

    /* If we already have enough terms, return the requested one. */
    if (vec && vec_len >= (size_t)n)
        return vec[n - 1];

    /* Rediscover / extend the sequence until at least n terms are present. */
    while (true) {
        /* Allocate working arrays */
        uint64_t *phi = (uint64_t*)malloc((L + 1) * sizeof(uint64_t));
        if (!phi) return (seq_t)0;              /* allocation failed */
        unsigned char *is_tot = (unsigned char*)calloc(L + 1, 1);
        unsigned char *is_cot = (unsigned char*)calloc(L + 1, 1);
        if (!is_tot || !is_cot) { free(phi); free(is_tot); free(is_cot); return (seq_t)0; }

        /* initialise φ values */
        for (uint64_t i = 0; i <= L; ++i) phi[i] = i;

        /* sieve for Euler's totient */
        for (uint64_t p = 2; p <= L; ++p) {
            if (phi[p] == p) {                 /* p is prime */
                for (uint64_t j = p; j <= L; j += p) {
                    phi[j] = phi[j] / p * (p - 1);
                }
            }
        }

        /* mark totients and cototients */
        for (uint64_t i = 2; i <= L; ++i) {
            uint64_t t = phi[i];
            if (t <= L) is_tot[t] = 1;
            uint64_t c = i - t;                 /* cototient */
            if (c <= L) is_cot[c] = 1;
        }

        /* count how many intersection values we have so far */
        size_t count = 0;
        for (uint64_t v = 1; v <= L; ++v)
            if (is_tot[v] && is_cot[v]) ++count;

        if (count >= (size_t)n) {               /* enough terms: build vector */
            /* free old vector if present */
            if (vec) free(vec);

            vec = (seq_t*)malloc(count * sizeof(seq_t));
            if (!vec) {
                free(phi); free(is_tot); free(is_cot);
                return (seq_t)0;                 /* allocation failed */
            }
            size_t idx = 0;
            for (uint64_t v = 1; v <= L; ++v)
                if (is_tot[v] && is_cot[v])
                    vec[idx++] = (seq_t)v;        /* store ascending */

            vec_len = count;

            /* cleanup temporary arrays */
            free(phi); free(is_tot); free(is_cot);
            break;                                 /* we have the answer */
        }

        /* Not enough terms: clean up and increase the limit. */
        free(phi); free(is_tot); free(is_cot);
        L *= 2ULL;                                /* double the range and retry */
    }

    /* Now vec_len >= n */
    return vec[n - 1];
}
