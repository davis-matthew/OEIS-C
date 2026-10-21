/*
 * A054817  Fifth term of strong prime sextets: p(m-3)-p(m-4) > p(m-2)-p(m-3)
 *          > p(m-1)-p(m-2) > p(m)-p(m-1) > p(m+1)-p(m).
 * https://oeis.org/A054817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Fifth term of strong prime sextets (OEIS A054817)
   Computed by generating all primes up to a fixed limit
   (30 000 000) and storing the 5th prime of every sextet
   with strictly decreasing gaps.
   a(0) = 1871, a(1) = 2539, … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --------------  Function required by the build system  -------------- */
seq_t a(idx_t n)
{
    /* ----------------------  Static data  --------------------------- */
    static bool          initialized = false;
    static seq_t*        terms       = NULL;   /* the sequence values   */
    static size_t        terms_cnt   = 0;      /* number of terms       */
    static size_t        terms_cap   = 0;      /* allocated capacity    */
    static uint32_t*     primes      = NULL;   /* list of primes        */
    static size_t        primes_cnt  = 0;      /* number of primes      */

    /* -----------------  First call initialization  ----------------- */
    if (!initialized)
    {
        /* ---- 1.  Sieve all primes up to LIMIT -------------------- */
        const size_t LIMIT = 30000000UL;   /* 30 million */

        size_t i, j;
        size_t sieve_size = LIMIT + 1;
        /* boolean array: 0 = prime candidate, 1 = composite */
        char *isComposite = (char*)malloc(sieve_size);
        if (!isComposite) { return 0; }
        memset(isComposite, 0, sieve_size);

        for (i = 2; i * i <= LIMIT; ++i)
        {
            if (!isComposite[i])
            {
                for (j = i * i; j <= LIMIT; j += i)
                    isComposite[j] = 1;
            }
        }

        /* ---- 2.  Collect primes into a dynamic array -------------- */
        /* π(30 M) ≈ 1.85 million, 2 million capacity is safe           */
        const size_t PRIME_CAP = 2000000UL;
        primes = (uint32_t*)malloc(PRIME_CAP * sizeof(uint32_t));
        if (!primes) { free(isComposite); return 0; }

        primes_cnt = 0;
        for (i = 2; i <= LIMIT; ++i)
        {
            if (!isComposite[i])
            {
                if (primes_cnt == PRIME_CAP) { free(isComposite); free(primes); return 0; }
                primes[primes_cnt++] = (uint32_t)i;
            }
        }

        /* ---- 3.  Scan all consecutive sextets --------------------- */
        terms_cap = 1024;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        if (!terms) { free(isComposite); free(primes); return 0; }
        terms_cnt = 0;

        for (size_t idx = 0; idx + 5 < primes_cnt; ++idx)
        {
            uint32_t d1 = primes[idx + 1] - primes[idx];
            uint32_t d2 = primes[idx + 2] - primes[idx + 1];
            uint32_t d3 = primes[idx + 3] - primes[idx + 2];
            uint32_t d4 = primes[idx + 4] - primes[idx + 3];
            uint32_t d5 = primes[idx + 5] - primes[idx + 4];

            if (d1 > d2 && d2 > d3 && d3 > d4 && d4 > d5)
            {
                /* grow terms array if needed */
                if (terms_cnt == terms_cap)
                {
                    terms_cap <<= 1;
                    seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) { free(isComposite); free(primes); free(terms); return 0; }
                    terms = tmp;
                }
                /* store the 5th prime of this sextet */
                terms[terms_cnt++] = (seq_t)primes[idx + 4];
            }
        }

        /* ---- 4.  Clean up temporary data -------------------------- */
        free(isComposite);
        free(primes);      /* primes are no longer needed */

        /* ---- 5.  Mark as initialized ----------------------------- */
        initialized = true;
    }

    /* -----------------  Return requested term  ------------------- */
    if ((idx_t)terms_cnt > n)
        return terms[(size_t)n];

    /* index out of range – undefined for this build */
    return 0;
}
