/*
 * A156071  Concatenation chain arising in A156069.
 * https://oeis.org/A156071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Algorithm:  The sequence is the polydivisible chain
   starting with 3.  For k = 2 … 9 we use digits from 1..9
   not yet used; we choose the smallest digit that lets the
   chain be extended to length 9 (a well‑known pandigital
   381654729).  For k ≥10 we simply append the smallest digit
   0..9 that makes the new number divisible by its new length.
   The routine builds the first 9 terms by backtracking; all
   further terms are obtained greedily, which reproduces the
   OEIS sequence.
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cache_size = 0;    /* number of terms stored */
static idx_t  cache_cap   = 0;    /* allocated capacity */

/* ------------------------------------------------------------------ */
/* Grow the cache to at least 'needed' elements.                      */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Build the unique 9‑digit polydivisible chain 3 38 381 ... 729.      */
/* The recursion stops as soon as the full chain is found.              */
static seq_t chain9[9];

/* backtracking helper for the first 9 terms                          */
static int dfs_build(unsigned int used_mask, idx_t len, seq_t current)
{
    if (len == 9) return 1;              /* full length reached */

    for (int d = 1; d <= 9; ++d) {       /* digits 1‑9 only */
        if (used_mask & (1u << d)) continue;
        idx_t new_len = len + 1;
        /* check (current*10 + d) % new_len == 0 */
        seq_t rem = ((current % (seq_t)new_len) * 10 + d) % (seq_t)new_len;
        if (rem != 0) continue;
        chain9[new_len - 1] = current * 10 + d;
        if (dfs_build(used_mask | (1u << d), new_len, chain9[new_len - 1]))
            return 1;                    /* stop on first complete chain */
    }
    return 0;                            /* no extension possible */
}

/* Ensure the cache contains the first nine terms.                    */
static void init_chain9(void)
{
    chain9[0] = (seq_t)3;
    dfs_build(1u << 3, /* len=1, used 3 */ 1, 3);
    grow_cache(9);
    for (idx_t i = 0; i < 9; ++i) cache[i] = chain9[i];
    cache_size = 9;
}

/* ------------------------------------------------------------------ */
/* public required function                                            */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset < 1 */

    if (cache_size == 0) init_chain9();   /* build the base chain */

    if (n <= cache_size) return cache[n - 1];

    /* extend the cache up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        grow_cache(i);
        seq_t prev = cache[cache_size - 1];
        seq_t next = 0;
        /* minimal digit 0‑9 that makes prev*10 + d divisible by i */
        for (int d = 0; d <= 9; ++d) {
            seq_t rem = ((prev % (seq_t)i) * 10 + d) % (seq_t)i;
            if (rem == 0) {
                next = prev * 10 + d;
                break;
            }
        }
        cache[cache_size] = next;
        ++cache_size;
    }
    return cache[n - 1];
}
