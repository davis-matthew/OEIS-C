/*
 * A106840  Numbers m such that both m and m+1 have odd part of the form
 *          4*k+1.
 * https://oeis.org/A106840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106840: Numbers m such that both m and m+1 have odd part ≡ 1 (mod 4) */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/odd_part.h"

/* Memoised storage for generated terms. */
static seq_t *terms       = NULL;   /* array of terms */
static idx_t terms_count  = 0;      /* number of terms currently stored */
static idx_t terms_cap    = 0;      /* capacity of the array */
static seq_t last_checked = 0;      /* largest m that has been examined */

/* Ensure the terms array can hold at least 'min' elements. */
static void ensure_capacity(idx_t min)
{
    if (terms_cap >= min)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    while (new_cap < (size_t)min)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();  /* allocation failure */
    terms          = tmp;
    terms_cap      = (idx_t)new_cap;
}

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n==0 */

    /* Generate terms until we have at least n of them. */
    while (terms_count < n) {
        ensure_capacity(terms_count + 1);

        seq_t m = last_checked + 1;
        if (((odd_part(m)          & 3) == 1) &&
            ((odd_part(m + 1)      & 3) == 1)) {
            terms[terms_count++] = m;
        }
        last_checked = m;
    }

    return terms[n - 1];
}
