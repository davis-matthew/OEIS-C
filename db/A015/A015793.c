/*
 * A015793  Numbers n such that phi(n) + 4 divides sigma(n).
 * https://oeis.org/A015793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1300, u32 n <= 1300, u64 n <= 1300,
 *            u128 n <= 1300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that phi(n) + 4 divides sigma(n) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The function that OEIS calls */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *terms   = NULL;
    static size_t terms_cap   = 0;
    static size_t terms_count = 0;
    static idx_t  cache_limit = 0;

    /* If we already have the requested term, return it */
    if (n <= terms_count)
        return terms[n-1];

    /* Grow the cache until it contains at least n terms */
    while (n > terms_count) {
        /* Determine a new upper bound for sieving.
           Start reasonably large and double if necessary. */
        idx_t new_limit = cache_limit ? cache_limit * 2 : 50000;
        if (new_limit < n*20) new_limit = n*20;
        if (new_limit < 50000) new_limit = 50000;

        /* Allocate arrays for smallest prime factor */
        uint64_t *spf = (uint64_t*)malloc((new_limit+1) * sizeof(uint64_t));
        if (!spf) return 0;                 /* Out of memory */
        for (idx_t i = 0; i <= new_limit; ++i) spf[i] = 0;
        spf[0] = 0; spf[1] = 1;

        /* Simple sieve to fill spf */
        for (idx_t i = 2; i <= new_limit; ++i) {
            if (spf[i] == 0) {              /* i is prime */
                for (idx_t j = i; j <= new_limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }

        /* Temporary storage for terms from this pass */
        seq_t *new_terms = (seq_t*)realloc(terms, (terms_count + new_limit/5) * sizeof(seq_t));
        if (!new_terms) {
            free(spf);
            return 0;
        }
        terms = new_terms;
        if (terms_cap < terms_count + new_limit/5)
            terms_cap = terms_count + new_limit/5;

        /* Scan numbers up to new_limit, computing phi & sigma on the fly. */
        for (idx_t num = 2; num <= new_limit; ++num) {
            /* Skip numbers already processed in previous passes */
            if (num <= cache_limit) continue;

            /* Compute φ(num) and σ(num) using the precomputed spf */
            seq_t phi   = (seq_t)num;
            seq_t sigma = 1;
            uint64_t t = num;

            while (t > 1) {
                uint64_t p = spf[t];
                int exp = 0;
                seq_t pp = 1; /* p^exp */
                while (t % p == 0) {
                    t /= p;
                    ++exp;
                    pp *= p;
                }
                /* Update φ: φ(n) *= (p-1) / p for each distinct p */
                phi = phi / p * (p-1);

                /* Compute factor for σ:
                   sum_{k=0}^{exp} p^k  = (p^{exp+1}-1)/(p-1)  */
                seq_t term = 1;          /* p^0 */
                seq_t cur = 1;           /* p^k */
                for (int k = 1; k <= exp; ++k) {
                    cur *= p;
                    term += cur;
                }
                sigma *= term;
            }

            /* Check the divisibility condition */
            seq_t divisor = phi + (seq_t)4;
            if (divisor != 0 && sigma % divisor == 0) {
                if (terms_count >= terms_cap) {
                    terms_cap *= 2;
                    seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) {
                        free(spf);
                        return 0;
                    }
                    terms = tmp;
                }
                terms[terms_count++] = (seq_t)num;
            }
        }

        free(spf);
        cache_limit = new_limit;
    }

    /* Return the cached term */
    return terms[n-1];
}
