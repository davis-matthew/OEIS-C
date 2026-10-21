/*
 * A076144  Largest squarefree m <= sfn(n) such that m*sfn(n) is also
 *          squarefree, where sfn(n) is the n-th squarefree number.
 * https://oeis.org/A076144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest squarefree m ≤ sfn(n) such that gcd(m,sfn(n)) =
 *            1, where sfn(n) is the n‑th squarefree number
 * Range:     u8 n <= 157, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/gcd.h"

/* cache of squarefree numbers */
static seq_t *sfn_cache  = NULL;
static size_t sfn_cap    = 0;
static size_t sfn_len    = 0;
static idx_t  current_val = 1;   /* next integer to test for squarefreeness */

/* enlarge cache until it contains at least 'target' squarefrees */
static void _ensure_sfn(idx_t target)
{
    if (sfn_len >= target) return;

    /* grow capacity if needed */
    size_t need = target + 128;
    if (need > sfn_cap) {
        size_t new_cap = sfn_cap ? sfn_cap * 2 : 128;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(sfn_cache, new_cap * sizeof(seq_t));
        sfn_cache = tmp;
        sfn_cap   = new_cap;
    }

    /* append new squarefree numbers until we reach the target length */
    while (sfn_len < target) {
        if (is_squarefree((seq_t)current_val))
            sfn_cache[sfn_len++] = current_val;
        ++current_val;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for n < 1, guard only */
        return (seq_t)0;
    if (n == 1)                  /* a(1) = 1 by definition */
        return (seq_t)1;

    _ensure_sfn(n);              /* need sfn(1)…sfn(n) */

    seq_t S = sfn_cache[n-1];    /* sfn(n) */
    idx_t i = n-2;               /* start from sfn(n-1) */

    while (1) {
        seq_t cand = sfn_cache[i];
        if (gcd(cand, S) == (seq_t)1)
            return cand;
        if (i == 0)
            break;
        --i;
    }
    return (seq_t)1;             /* fallback – should never be reached */
}
