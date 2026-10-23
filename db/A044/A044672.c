/*
 * A044672  Numbers n such that string 4,4 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_44
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044672:  n-th positive integer whose base‑9 representation
   contains the substring 44, but the base‑9 representation of n+1
   does NOT contain 44.  */

/* Note: the implementation scans successive integers, stores all
   numbers satisfying the condition in a dynamic array, and returns
   the n‑th element of this array.  The array is built on first use
   and extended when larger n are requested. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_44.h"

static seq_t *memo = NULL;          /* array of found terms            */
static idx_t  memo_cnt = 0;         /* number of terms currently stored*/
static size_t memo_cap = 0;         /* current allocated capacity      */
static seq_t  next_candidate = 1;   /* next integer to test           */

/* Extend the memo array until it contains at least 'target' terms. */
static void ensure_terms(idx_t target)
{
    while (memo_cnt < target)
    {
        seq_t cand   = next_candidate;
        seq_t nextv  = cand + 1;
        bool ok = has_44(cand) && !has_44(nextv);
        ++next_candidate;

        if (ok)
        {
            if (memo == NULL)
            {
                memo_cap = 16;
                memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
            }
            else if ((size_t)(memo_cnt + 1) > memo_cap)
            {
                memo_cap *= 2;
                memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
            }
            memo[memo_cnt++] = cand;
        }
    }
}

/* Public function required by the build system.  */
seq_t a(idx_t n)
{
    /* OEIS offset is 1; we ignore n == 0 (undefined). */
    if (n == 0)
        return 0;
    ensure_terms(n);
    return memo[n - 1];
}
