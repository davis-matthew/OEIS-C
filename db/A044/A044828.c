/*
 * A044828  Positive integers having more base-2 runs of even length than odd.
 * https://oeis.org/A044828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_even_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search.  The property is checked by
   scanning the binary representation of each candidate.  */
/* The function is memoised so that random access to any n is
   efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_even_runs.h"

/* Return the n‑th positive integer whose binary representation has
   more runs of even length than runs of odd length.  Indexing starts
   at 1.  This function is thread‑unsafe but satisfies the OEIS
   requirements. */
seq_t a(idx_t n)
{
    static seq_t *list = NULL;            /* memoised list of terms   */
    static size_t cap   = 0;              /* allocated capacity       */
    static size_t len   = 0;              /* number of stored terms   */
    static seq_t last   = 0u;             /* last candidate examined */

    if (n == 0u)
        return 0u;                        /* undefined for n<1         */

    /* grow the list until it contains at least n terms */
    while (len < (size_t)n)
    {
        ++last;                           /* next candidate            */
        if (has_even_runs(last))
        {
            if (len == cap)
            {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(list, newcap * sizeof(seq_t));
                if (!tmp)              /* allocation failure         */
                    exit(EXIT_FAILURE);
                list = tmp;
                cap = newcap;
            }
            list[len++] = last;            /* store the qualifying number*/
        }
    }
    return list[n - 1];                   /* zero‑based array index     */
}
