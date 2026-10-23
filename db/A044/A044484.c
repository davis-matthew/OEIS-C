/*
 * A044484  Numbers n such that string 0,1 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   contains_adj_adj01
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th integer > 0 whose base‑6 expansion
 * contains the adjacent digits 0,1 but whose successor n+1 does NOT.
 *
 * The implementation performs a brute‑force search with
 * memoisation in a static cache.  Each candidate integer is tested
 * by examining its base‑6 digits from least to most significant.
 * Complexity is O(number of tested integers * log_6(n)), fast for
 * n up to 10000 (few hundred thousand candidates).
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/contains_adj_adj01.h"

/* A number qualifies if its own base‑6 representation contains
   0,1 but the representation of its successor does not. */
static bool qualifies(seq_t n)
{
    return contains_adj_adj01(n) && !contains_adj_adj01(n + (seq_t)1);
}

/* Main sequence routine.  Uses a static cache for memoisation.
 * a(1) is the first term of the OEIS list. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for offset 0 */

    /* Static cache: index 1 -> cache[0] */
    static seq_t *cache = NULL;
    static size_t cache_cnt = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity          */
    static seq_t next_val = (seq_t)1;  /* smallest candidate to test */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_cnt)
        return cache[(size_t)n - 1];

    /* Generate terms until we have at least n of them. */
    while (cache_cnt < (size_t)n) {
        if (qualifies(next_val)) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)          /* memory exhausted */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = next_val;
        }
        ++next_val;
    }

    return cache[(size_t)n - 1];
}
