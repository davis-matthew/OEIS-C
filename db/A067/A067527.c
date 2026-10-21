/*
 * A067527  n - 3^k is a prime for all k > 0 such that 3^k < n.
 * https://oeis.org/A067527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: n - 3^k is prime for all k>0 with 3^k < n.
   Computes terms by brute‑force search.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Static storage for memoised terms.  */
static seq_t *terms = NULL;       /* array of stored terms          */
static size_t terms_len = 0;      /* number of terms stored         */
static size_t terms_cap = 0;      /* array capacity                 */
static uint64_t checked_upto = 0; /* largest integer examined so far */

/* Ensure the array has room for at least 'need' elements. */
static void grow_terms(size_t need)
{
    if (need <= terms_cap)
        return;

    size_t new_cap = terms_cap ? terms_cap * 2 : 4;
    while (new_cap < need)
        new_cap *= 2;

#ifdef __cplusplus
    /* C++: allocate with new and copy existing elements. */
    seq_t *new_terms = new seq_t[new_cap];
    for (size_t i = 0; i < terms_len; ++i)
        new_terms[i] = terms[i];
    delete[] terms;
    terms = new_terms;
#else
    /* C: realloc the raw memory block. */
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();   /* allocation failure */
    terms = new_terms;
#endif
    terms_cap = new_cap;
}

/* Test whether a candidate n satisfies the definition: n - 3^k must be prime for every 3^k < n. */
static bool candidate_ok(uint64_t n)
{
    uint64_t power = 3;
    while (power < n) {
        uint64_t d = n - power;
        if (!is_prime((idx_t)d))
            return false;
        if (power > UINT64_MAX / 3)
            break; /* overflow; next power would be >= n */
        power *= 3;
    }
    return true;
}

/* Generate terms until we have at least 'goal' terms. */
static void generate_terms(size_t goal)
{
    if (terms_len >= goal)
        return;

    /* Initialise with the first known term if the table is empty. */
    if (terms_len == 0) {
        grow_terms(1);
        terms[0] = (seq_t)5;        /* a(1) = 5 */
        terms_len = 1;
        checked_upto = 5;
    }

    uint64_t cand = checked_upto;
    while (terms_len < goal) {
        cand++;                       /* try next integer */
        if (candidate_ok(cand)) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)cand;
        }
    }
    checked_upto = cand;
}

/* Public function: a(n) returns the nth term (offset = 1).  */
seq_t a(idx_t n)
{
    if ((idx_t)n < 1)
        return (seq_t)0;              /* undefined for n < 1 */
    generate_terms((size_t)n);
    return terms[(size_t)(n - 1)];
}
