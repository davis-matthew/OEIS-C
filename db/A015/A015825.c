/*
 * A015825  Numbers k such that phi(k + 8) | sigma(k).
 * https://oeis.org/A015825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 117, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   alloc_seq_array, calc_phi, free_seq_array, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015825: Numbers k such that φ(k + 8) divides σ(k) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"     /* seq_t calc_phi(seq_t m); */
#include "../utils/sigma_of.h"    /* seq_t sigma_of(idx_t n); */
#include "../utils/alloc_seq_array.h" /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/free_seq_array.h"  /* void free_seq_array(seq_t *p); */

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *terms = NULL; /* array holding the found k values */
    static size_t count  = 0;   /* number of terms currently stored   */
    static size_t cap    = 0;   /* allocated capacity of 'terms'      */
    static uint64_t last_k = 0;/* largest k that has already been examined */

    if (n == 0) return 0;      /* offset is 1 – undefined for n==0 */

    /* generate terms until we have the n‑th one */
    while (count < n)
    {
        /* next candidate k */
        ++last_k;

        /* compute φ(k + 8) and σ(k) using the provided helpers */
        seq_t phi   = calc_phi((seq_t)(last_k + 8));
        seq_t sigma = sigma_of((idx_t)last_k);

        /* if σ(k) is divisible by φ(k + 8), keep this k */
        if (sigma % phi == 0)
        {
            /* grow the array if needed */
            if (count == cap)
            {
                size_t new_cap = (cap == 0) ? 64 : cap * 2;
                seq_t *tmp = alloc_seq_array(new_cap);
                if (!tmp) return 0; /* allocation failure */

                for (size_t i = 0; i < count; ++i)
                    tmp[i] = terms[i];
                if (terms)
                    free_seq_array(terms);

                terms = tmp;
                cap   = new_cap;
            }
            terms[count++] = (seq_t)last_k;
        }
    }

    return terms[n - 1];
}
