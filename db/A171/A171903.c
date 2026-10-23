/*
 * A171903  Numbers m such that m and m+1 have at least two identical
 *          neighboring digits in their decimal representations.
 * https://oeis.org/A171903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = the n‑th smallest integer m such that m and m+1 each
 *            contain at least one pair of adjacent identical decimal digits.
 *            The terms are listed in increasing order
 * Range:     u8 n <= 20, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Return true iff x has at least one pair of identical adjacent decimal digits. */
static bool has_adj_pair(seq_t x)
{
    if (x < ((seq_t)10))
        return false;
    seq_t prev = x % ((seq_t)10);
    x /= ((seq_t)10);
    while (x > ((seq_t)0)) {
        seq_t cur = x % ((seq_t)10);
        if (cur == prev)
            return true;
        prev = cur;
        x   /= ((seq_t)10);
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static state for memoization and continuation of search. */
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t  len   = 0;     /* number of terms stored */
    static size_t  cap   = 0;     /* array capacity (in terms) */
    static seq_t   cur   = ((seq_t)0);   /* last integer tested */

    if (n == (idx_t)0)          /* undefined for n == 0; return 0 (not used) */
        return (seq_t)0;

    /* Generate terms until we have at least n stored. */
    while (len < (size_t)n) {
        cur++;                       /* next candidate */

        if (has_adj_pair(cur) && has_adj_pair(cur + ((seq_t)1))) {
            /* Ensure capacity for another term. */
            if (len == cap) {
                cap = cap ? cap * 2 : 4;
                cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = cur;      /* store found term */
        }
    }

    return cache[(size_t)n - 1];
}
