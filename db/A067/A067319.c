/*
 * A067319  Numbers n such that phi(n)^phi(n)+1 is prime.
 * https://oeis.org/A067319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the nth integer n such that φ(n)^φ(n)+1 is prime,
 *            i.e. φ(n) ∈ {1, 2, 4}. The sequence is finite:
 *            1,2,3,4,5,6,8,10,12
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"  /* to use calc_phi(seq_t m) */

static seq_t *terms = NULL;          /* array of found terms */
static size_t terms_len = 0;         /* number of terms stored */
static idx_t   next_to_check = 1;    /* smallest n not yet examined */
static size_t  terms_cap = 0;        /* capacity of terms array */

static void ensure_terms_capacity(void)
{
    if (terms_len + 1 > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, sizeof(seq_t) * new_cap);
        if (tmp) {
            terms = tmp;
            terms_cap = new_cap;
        }
    }
}

/* We know the sequence stops after 12; scanning beyond that is futile.
   A conservative upper bound is set to 1000, far above 12. */
#define MAX_SCAN 1000

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for offset 1 */

    /* If already computed, return the cached value. */
    if (n <= terms_len)
        return terms[n - 1];

    /* Scan forward until we have the required number of terms or
       we hit the predetermined upper bound. */
    for (; next_to_check <= MAX_SCAN && terms_len < n; ++next_to_check) {
        seq_t phival = calc_phi((seq_t)next_to_check);
        if (phival == 1 || phival == 2 || phival == 4) {
            ensure_terms_capacity();
            terms[terms_len++] = (seq_t)next_to_check;
        }
    }

    /* If we still don't have enough terms, the requested index is out of range. */
    if (n <= terms_len)
        return terms[n - 1];
    return (seq_t)0;   /* sequence terminates after the known terms */
}
