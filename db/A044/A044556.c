/*
 * A044556  Numbers n such that string 5,2 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has52
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A044556: numbers n such that the string "52" occurs in the base-7
 * representation of n but not in the base-7 representation of n+1.
 *
 * Implementation: sequential search with memoization.  For each candidate
 * number m, test the property and store the qualifiers.  Results are
 * cached so that later calls to a(n) (in any order) can reuse already
 * discovered terms.
 *
 * The helper function has52() examines whether a base-7 representation
 * contains the adjacent digits 5 then 2.
 */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has52.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t cap = 0;          /* allocated capacity          */
    static idx_t num_terms = 0;     /* number of terms cached      */
    static idx_t last_checked = 0;  /* last integer examined       */

    if (n == 0) return 0;          /* offset is 1; undefined for 0 */

    /* extend cache until we have at least n terms */
    while (num_terms < n)
    {
        ++last_checked;            /* candidate integer m */
        seq_t m = (seq_t)last_checked;
        if (has52(m) && !has52(m + 1))
        {
            if (num_terms == cap)
            {
                size_t new_cap = cap == 0 ? 32 : cap * 2;
                terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                cap = new_cap;
            }
            terms[num_terms++] = m;
        }
    }
    return terms[n - 1];
}
