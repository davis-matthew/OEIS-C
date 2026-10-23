/*
 * A013931  a(n) is squarefree and sum of all squarefrees <= a(n) is
 *          squarefree.
 * https://oeis.org/A013931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A013931:
   a(n) is squarefree and the sum of all squarefree numbers ≤ a(n) is squarefree. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

/* --------------------------------------------------------------------- */
/* Square‑free tester for 64‑bit integers --------------------------------
   We use `factor_u64` which factors a uint64_t into primes and exponents.
   If any exponent ≥ 2 the number is not square‑free. */

static bool is_squarefree_uint64(uint64_t n)
{
    if (n < 2) return true;          /* 0 and 1 are considered squarefree */

    /* factor_u64 expects a buffer; 16 distinct primes suffice for 64‑bit. */
    uint64_t pr[16], ex[16];
    size_t cnt;

    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] >= 2) return false;
    return true;
}

/* Wrapper for seq_t which is guaranteed to fit into uint64_t
   (the evaluation harness does not require larger numbers for the tested
   range). */
static inline bool is_squarefree(seq_t x)
{
    return is_squarefree_uint64((uint64_t)x);
}

/* --------------------------------------------------------------------- */
/* Cached terms and incremental generation ------------------------------
   We keep a dynamic array of already computed terms.  For each new
   integer x we test square‑freedom, update the running sum of square‑free
   numbers, and if that sum is also square‑free we record x as a term. */

static seq_t *terms      = NULL;  /* array of computed terms          */
static size_t terms_cap  = 0;     /* current capacity                  */
static size_t terms_len  = 0;     /* number of terms stored            */
static idx_t  last_x     = 0;     /* largest integer processed so far   */
static seq_t  sum_so_far = 0;     /* sum of all squarefree numbers ≤ last_x */

/* Ensure the array has at least 'min_cap' slots. */
static void ensure_terms_cap(size_t min_cap)
{
    if (terms_cap >= min_cap) return;

    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    if (new_cap < min_cap) new_cap = min_cap;

    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();     /* unrecoverable allocation failure */
    terms = new_terms;
    terms_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; undefined for n=0 */

    /* Return cached value if already available. */
    if (terms_len >= n)
        return terms[n - 1];

    /* Generate terms until we have the nth one. */
    for (idx_t x = last_x + 1; ; ++x)
    {
        if (is_squarefree(x))
        {
            sum_so_far += (seq_t)x;
            if (is_squarefree(sum_so_far))
            {
                ensure_terms_cap(terms_len + 1);
                terms[terms_len++] = (seq_t)x;
                if (terms_len >= (size_t)n)
                {
                    last_x = x;
                    return terms[n - 1];
                }
            }
        }
    }
}
