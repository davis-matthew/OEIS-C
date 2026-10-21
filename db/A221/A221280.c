/*
 * A221280  Numbers m such that lambda(m) = lambda(m+1), where lambda(n) =
 *          A008836(n) is the Liouville function.
 * https://oeis.org/A221280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that λ(m) = λ(m+1) (Liouville
 *            function)
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static storage for cached terms and the last limit used. */
    static seq_t *terms = NULL;
    static size_t terms_len   = 0;
    static size_t computed_limit = 0;

    if (n == 0) return 0;  /* not used, safety guard */

    /* Produce more terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        size_t limit;
        if (computed_limit == 0) {
            /* Initial guess: beyond the expected density of 1/2 */
            limit = (size_t)(n * 2 + 10);
            if (limit < 32) limit = 32;
        } else {
            /* Grow the sieve range. */
            limit = computed_limit * 2;
            if (limit < computed_limit + 10)
                limit = computed_limit + 10;
        }

        /* Build an array containing the smallest prime factor (SPF)
           for each integer up to limit+1. */
        size_t max = limit + 1;
        uint32_t *spf = (uint32_t *)malloc((max + 1) * sizeof(uint32_t));
        if (!spf)
            return 0;                    /* allocation failure – unlikely */

        for (size_t i = 0; i <= max; ++i)
            spf[i] = 0;
        for (size_t i = 2; i <= max; ++i) {
            if (spf[i] == 0) {
                spf[i] = (uint32_t)i;
                if (i * i <= max) {
                    for (size_t j = i * i; j <= max; j += i)
                        if (spf[j] == 0)
                            spf[j] = (uint32_t)i;
                }
            }
        }

        /* Compute the parity of Ω(k) for all k up to max. */
        uint8_t *parity = (uint8_t *)malloc((max + 1) * sizeof(uint8_t));
        if (!parity) {
            free(spf);
            return 0;
        }
        parity[1] = 0;                    /* Ω(1) = 0 */
        for (size_t i = 2; i <= max; ++i)
            parity[i] = parity[i / spf[i]] ^ 1;   /* toggle parity */

        /* Generate terms up to the current limit. */
        seq_t *new_terms = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
        if (!new_terms) {
            free(spf);
            free(parity);
            return 0;
        }
        size_t count = 0;
        for (size_t m = 1; m <= limit; ++m) {
            if (parity[m] == parity[m + 1])
                new_terms[count++] = (seq_t)m;
        }

        /* Replace old cache with the new one. */
        free(spf);
        free(parity);
        free(terms);
        terms       = new_terms;
        terms_len   = count;
        computed_limit = limit;
        /* Repeat if we still don't have enough terms. */
    }

    return terms[n - 1];
}
