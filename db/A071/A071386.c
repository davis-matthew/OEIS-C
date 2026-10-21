/*
 * A071386  Numbers k such that the cardinality of the set of solutions to
 *          phi(x) = k is odd.
 * https://oeis.org/A071386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that the number of solutions * of
 *            φ(x) = k is odd
 * Range:     u8 n <= 29, u16 n <= 4348, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all k for which the pre‑image count
 * of the Euler totient function has odd cardinality.  We compute all
 * φ(x) up to a fixed bound, accumulate the parity of each k,
 * then collect the k’s with odd counts.  The bound is chosen large
 * enough (3 000 000) to cover the first 10 000 sequence terms.
 * This runs well below two seconds for the required n. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define LIMIT 3000000

seq_t a(idx_t n)
{
    static int initialized = 0;
    static uint32_t *phi = NULL;
    static uint32_t *cnt = NULL;
    static seq_t  *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    if (!initialized)
    {
        /* Allocate and compute Euler's totient values up to LIMIT */
        phi = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        cnt = (uint32_t *)calloc((LIMIT + 1), sizeof(uint32_t));
        if (!phi || !cnt) return 0;   /* memory failure */

        for (size_t i = 0; i <= LIMIT; ++i) phi[i] = (uint32_t)i;

        for (size_t p = 2; p <= LIMIT; ++p)
            if (phi[p] == (uint32_t)p)          /* p is prime */
                for (size_t j = p; j <= LIMIT; j += p)
                    phi[j] -= phi[j] / (uint32_t)p;

        /* Count how many times each totient value occurs */
        for (size_t x = 1; x <= LIMIT; ++x)
        {
            uint32_t v = phi[x];
            if (v <= LIMIT) cnt[v]++;  /* v < x <= LIMIT always holds */
        }

        /* Collect k with odd pre‑image counts */
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) { free(phi); free(cnt); return 0; }

        for (size_t k = 1; k <= LIMIT; ++k)
            if (cnt[k] & 1U)
            {
                if (terms_len == terms_cap)
                {
                    terms_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) { free(phi); free(cnt); free(terms); return 0; }
                    terms = tmp;
                }
                terms[terms_len++] = (seq_t)k;
            }

        free(phi);
        free(cnt);
        initialized = 1;
    }

    if (n == 0 || (size_t)(n - 1) >= terms_len) return 0;
    return terms[n - 1];
}
