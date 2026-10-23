/*
 * A038352  Numbers whose base-6 representation has the same nonzero number of
 *          2's and 3's.
 * https://oeis.org/A038352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-6 representation has the same nonzero number
 *            of 2's and 3's
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   has_equal_nonzero23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_nonzero23.h"

/* The sequence cache ----------------------------------------------------- */
static seq_t *terms      = NULL;       /* array of found terms      */
static idx_t  terms_cnt  = 0;          /* number of terms stored    */
static idx_t  terms_cap  = 0;          /* allocated capacity        */
static seq_t  next_num   = 0;          /* next number to test       */

/* Ensure the cache has room for at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if (need <= terms_cap)
        return;
    idx_t new_cap = terms_cap == 0 ? 128 : terms_cap * 2;
    if (new_cap < need)
        new_cap = need;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Main sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least 'n' of them. */
    while (terms_cnt < n) {
        if (has_equal_nonzero23(next_num)) {
            ensure_capacity(terms_cnt + 1);
            terms[terms_cnt++] = next_num;
        }
        next_num++;
    }
    return terms[n - 1];
}
