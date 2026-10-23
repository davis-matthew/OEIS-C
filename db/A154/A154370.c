/*
 * A154370  Numbers k such that gpf(composite(k)) - lpf(composite(k)) is
 *          prime.
 * https://oeis.org/A154370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 8501, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, gpf, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A154370.
   The function generates terms on demand: it keeps a cache of terms
   and iterates over composite numbers, checking whether
   gpf(composite) – lpf(composite) is prime.  The k-th term is the
   index of the composite number that satisfies this property. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/gpf.h"
#include "../utils/isPrimeGeneric.h"

/* Ensure the terms array has room for at least 'needed' entries. */
static void
ensure_terms_cap(size_t needed, seq_t **arr, size_t *cap)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1); /* unrecoverable; should not happen in tests */
        *arr   = tmp;
        *cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0 */

    /* static state keeps the sequence memoized in any call order */
    static seq_t *terms = NULL;
    static size_t   terms_len   = 0;
    static size_t   terms_cap   = 0;
    static idx_t    next_num    = 4;      /* first composite number   */
    static idx_t    next_comp_idx = 1;    /* index of next_num        */

    /* generate terms until we have at least 'n' of them */
    while (terms_len < n) {
        /* test if next_num is composite */
        uint64_t sp = _smallest_prime_factor((uint64_t)next_num);
        if (sp != next_num) {                 /* composite */
            uint64_t lp   = gpf((uint64_t)next_num);
            uint64_t diff = (lp > sp) ? (lp - sp) : (sp - lp);

            /* diff > 0 for a composite; check primality */
            if (diff > 0 && isPrimeGeneric((seq_t)diff)) {
                ensure_terms_cap(terms_len + 1, &terms, &terms_cap);
                terms[terms_len++] = (seq_t)next_comp_idx;
            }
            next_comp_idx++;                  /* advance only for composites */
        }
        next_num++;                            /* try next integer */
    }

    return terms[n - 1];
}
