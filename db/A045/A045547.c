/*
 * A045547  Numbers whose factorial has '2' as its final nonzero digit.
 * https://oeis.org/A045547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: Numbers whose factorial has 2 as its final non‑zero digit.
   The function finds successive integers k such that the last
   non‑zero digit of k! is 2.  It is compliant with all
   specified build configurations.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/seq_realloc.h"   /* for seq_realloc(seq_t*,size_t,size_t) */

static seq_t *terms   = NULL;       /* dynamic array of computed terms   */
static size_t terms_sz  = 0;        /* number of terms stored           */
static size_t terms_cap = 0;        /* capacity of terms array          */
static idx_t  last_checked = 0;     /* value of k that has just been processed */
static uint64_t prod = 1;           /* k! with trailing zeros removed, kept modulo 10⁶ */

seq_t a(idx_t n)
{
    /* Generate the sequence until n terms are ready. */
    while (terms_sz < n)
    {
        ++last_checked;
        uint64_t mul = (uint64_t)last_checked;

        /* Multiply using 128‑bit arithmetic to avoid overflow. */
        __uint128_t tmp = (__uint128_t)prod * mul;
        prod = (uint64_t)tmp;

        /* Strip all trailing zeros. */
        while ((prod % 10ULL) == 0ULL)
            prod /= 10ULL;

        /* Keep the value small to preserve the last non‑zero digit. */
        prod %= 1000000ULL;

        if ((prod % 10ULL) == 2ULL)
        {
            if (terms_sz == terms_cap)
            {
                /* Grow the array.  Use seq_realloc so it works with
                   arbitrary‑precision seq_t classes. */
                size_t new_cap = terms_cap ? (terms_cap << 1) : 4;
                seq_t *tmp_terms = seq_realloc(terms,
                                               terms_cap,
                                               new_cap);
                if (!tmp_terms)
                {
                    /* Out of memory – abort cleanly. */
                    exit(EXIT_FAILURE);
                }
                terms      = tmp_terms;
                terms_cap  = new_cap;
            }
            terms[terms_sz++] = (seq_t)last_checked;
        }
    }
    return terms[n - 1];
}
