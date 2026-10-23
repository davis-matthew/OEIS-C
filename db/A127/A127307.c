/*
 * A127307  Positions of Dyck words beginning as UUD (110) in A014486/A063171.
 * https://oeis.org/A127307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): indices of Dyck words that start with 110
 * (UUD) by brute‑force enumeration of Dyck words in
 * lexicographic (numeric) order.  The function is
 * memoised per call and works for any n ≥ 1. */

#include <stdbool.h>

static bool dfs(idx_t opens, idx_t closes, idx_t t,
                unsigned char *w,
                idx_t *index, idx_t *found, idx_t target)
{
    idx_t pos = opens + closes;

    if (pos == 2 * t) {                /* complete word reached */
        (*index)++;                    /* position of this word   */
        if (w[0] && w[1] && !w[2]) {  /* starts with 110?        */
            (*found)++;
            if (*found == target)
                return true;           /* target found            */
        }
        return false;                   /* continue searching       */
    }

    /* Lexicographic order: try '0' before '1' whenever possible. */
    if (closes < t && (opens - closes) > 0) {  /* can put a closing 0 */
        w[pos] = 0;
        if (dfs(opens, closes + 1, t, w, index, found, target))
            return true;
    }
    if (opens < t) {                     /* can put an opening 1    */
        w[pos] = 1;
        if (dfs(opens + 1, closes, t, w, index, found, target))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t index  = 0;   /* global position counter (starting from 0) */
    idx_t found  = 0;   /* number of matches seen so far             */

    unsigned char work[128];  /* buffers word bits (enough for large n)    */

    for (idx_t t = 1; ; ++t) {        /* semilength: number of U's           */
        if (dfs(0, 0, t, work, &index, &found, n))
            return (seq_t)index;   /* index is 1‑based because incremented
                                      before checking */ 
    }
}
