/*
 * A069969  Numbers n such that phi(reverse(n)) = phi(reverse(n-1)) +
 *          phi(reverse(n-2)).
 * https://oeis.org/A069969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   mem, reverse_digits, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069969: numbers n such that
   φ(reverse(n)) = φ(reverse(n-1)) + φ(reverse(n-2)).
   The implementation searches increasingly for candidates and
   caches the found terms. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <stdbool.h>
#include "../utils/reverse_digits.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Cached terms of the sequence. */
static seq_t *terms = NULL;     /* array of terms      */
static idx_t  terms_len = 0;    /* number of terms stored */
static idx_t  terms_cap = 0;    /* capacity of the array */
static idx_t  last_candidate = 2; /* last integer tested */

/* Ensure the terms array can hold at least cap elements. */
static void ensure_terms_capacity(idx_t cap)
{
    if (terms_cap >= cap)
        return;

    seq_t *new_arr = alloc_seq_array(cap);
    if (!new_arr)
        abort();           /* allocation failure */

    for (idx_t i = 0; i < terms_len; ++i)
        new_arr[i] = terms[i];

    if (terms != NULL)
        mem_free(terms);
    terms = new_arr;
    terms_cap = cap;
}

/* Generate terms until we have at least n terms in the array. */
static void ensure_terms_to(idx_t n)
{
    /* grow capacities in powers of two for efficiency */
    if (terms_cap == 0)
        ensure_terms_capacity(8);

    while (terms_len < n)
    {
        idx_t cand = (terms_len == 0) ? 3 : last_candidate + 1;
        for (;; ++cand)
        {
            seq_t rev_cand   = reverse_digits((seq_t)cand);
            seq_t rev_m1     = reverse_digits((seq_t)(cand - 1));
            seq_t rev_m2     = reverse_digits((seq_t)(cand - 2));

            seq_t phi_cand = totient_seq(rev_cand);
            seq_t phi_m1   = totient_seq(rev_m1);
            seq_t phi_m2   = totient_seq(rev_m2);

            if (phi_cand == phi_m1 + phi_m2)
            {
                /* store the term */
                if (terms_len == terms_cap)
                    ensure_terms_capacity(terms_cap * 2);
                terms[terms_len++] = (seq_t)cand;
                last_candidate = cand;
                break;
            }
        }
    }
}

/* Public access function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* undefined for n < 1 */

    ensure_terms_to(n);
    return terms[n - 1];
}
