/*
 * A043221  Numbers k such that 0 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k (offset 1) whose decimal
 *            representation contains the adjacent digits 0 and 6 (either "06"
 *            or "60") while the integer k-1 does not contain such a
 *            substring. This implementation searches for the required terms
 *            incrementally and memoises the result so that repeated calls
 *            a...
 * Range:     u8 n <= 4, u16 n <= 1280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9887 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x contains "06" or "60" in its decimal representation. */
static bool has06_or60(seq_t x)
{
    const seq_t ten = (seq_t)10;
    if (x < ten) return false;
    seq_t prev = x % ten;
    x /= ten;
    while (x > 0)
    {
        seq_t cur = x % ten;
        if ((prev == (seq_t)0 && cur == (seq_t)6) ||
            (prev == (seq_t)6 && cur == (seq_t)0))
            return true;
        prev = cur;
        x /= ten;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Memoised table of terms */
    static seq_t *terms = NULL;
    static idx_t  terms_cnt = 0;      /* number of stored terms */
    static seq_t  search_start = 0;   /* next integer to test */

    /* Initialise on first call */
    if (terms == NULL)
    {
        terms_cnt = 1;
        terms = mem_new(seq_t, 1);
        terms[0] = (seq_t)60;         /* a(1) = 60 */
        search_start = (seq_t)61;     /* start searching after the first term */
    }

    /* Generate terms until we have at least n of them */
    while (terms_cnt < n)
    {
        seq_t k = search_start;
        while (1)
        {
            if (has06_or60(k))
            {
                seq_t km1 = k - (seq_t)1;
                if (!has06_or60(km1))
                    break;           /* k is the next required term */
            }
            ++k;
        }

        /* Store the found term */
        seq_t *tmp = mem_resize(seq_t, terms, (terms_cnt + 1));
        if (tmp == NULL)              /* allocation failure – abort */
            abort();
        terms = tmp;
        terms[terms_cnt++] = k;
        search_start = k + (seq_t)1;   /* continue past this value next time */
    }

    return terms[n - 1];               /* zero‑based index for the array */
}
