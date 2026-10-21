/*
 * A097620  Numbers having more prime factors than their neighbors together.
 * https://oeis.org/A097620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m for which Ω(m) > Ω(m-1)+Ω(m+1).
 *            Computed by sieve and filtering
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* ----- lazy initialization ------------------------------------------ */
    static int ready = 0;
    static seq_t *terms = NULL;
    static size_t terms_size  = 0;
    static size_t terms_cap   = 0;
    const idx_t LIMIT = 2000000;            /* sufficient for ~10 k terms */

    if (!ready)
    {
        /* Allocate temporary arrays for the linear sieve */
        uint32_t *spf   = (uint32_t *)calloc((size_t)LIMIT + 1U, sizeof(uint32_t));
        uint8_t  *omega = (uint8_t  *)calloc((size_t)LIMIT + 1U, sizeof(uint8_t));
        if (!spf || !omega) { free(spf); free(omega); return 0; }

        /* Linear sieve */
        uint32_t primes[150000];
        size_t   pcount = 0;
        omega[1] = 0;
        for (idx_t i = 2; i <= LIMIT; ++i)
        {
            if (spf[(size_t)i] == 0) {
                spf[(size_t)i] = (uint32_t)i;
                primes[pcount++] = (uint32_t)i;
                omega[(size_t)i] = 1;
            }
            for (size_t pj = 0; pj < pcount; ++pj) {
                uint32_t p = primes[pj];
                idx_t v = i * (idx_t)p;
                if (v > LIMIT) break;
                spf[(size_t)v] = p;
                omega[(size_t)v] = omega[(size_t)i] + 1;
                if (p == spf[(size_t)i]) break;
            }
        }

        /* Build the terms array */
        terms_cap = 10000;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) { free(spf); free(omega); return 0; }

        for (idx_t k = 2; k + 1 <= LIMIT; ++k) {
            if ( (uint32_t)omega[(size_t)k] > (uint32_t)(omega[(size_t)k-1] + omega[(size_t)k+1]) ) {
                if (terms_size == terms_cap) {
                    size_t new_cap = terms_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) { free(spf); free(omega); free(terms); return 0; }
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_size++] = (seq_t)k;
            }
        }

        free(spf);
        free(omega);
        ready = 1;
    }

    /* ----- return requested term --------------------------------------- */
    if (n == 0 || n > terms_size) return 0;
    return terms[n - 1];
}
