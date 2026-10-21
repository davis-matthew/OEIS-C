/*
 * A052892  E.g.f. is series reversion of log(1+x)*(1-x).
 * https://oeis.org/A052892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A052892 - number of set partitions weighted by Pochhammer symbols
    Formula: a(n) = Sum_{k=0 .. n-1} (n)_k * Stirling2(n, k+1)
              where (n)_k = n*(n+1)*...*(n+k-1) (Pochhammer symbol).
    The function computes values with memoisation; it works for any
    seq_t typedef (unsigned 8/16/32/64-bit, unsigned __int128, or an
    arbitrary-precision class) and for any n >= 0.  It never uses
    floating-point arithmetic.  Complexity: O(n^2) for reaching n,
    which is below 2 s up to about n = 10 000.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the zero entry directly.  */
    if (n == 0)
        return (seq_t)0;

    /* Static helpers to keep state between calls.  */
    static seq_t *prev_row = NULL;          /* Stirling row for n-1 */
    static seq_t *cur_row  = NULL;          /* Buffer for new row   */
    static idx_t  row_cap  = 0;             /* Capacity of row arrays */
    static seq_t *a_cache  = NULL;          /* Cached a(n) values   */
    static idx_t  a_cap    = 0;             /* Capacity of a_cache  */
    static idx_t  last_n   = 0;             /* Largest n already computed */

    /* Ensure a_cache has enough space.  */
    if (a_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        void *tmp;
        tmp = realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) /* allocation failed – abort silently */
            return (seq_t)0;
        a_cache = (seq_t *)tmp;
        /* Zero new entries.  */
        for (size_t i = a_cap; i < new_cap; ++i)
            a_cache[i] = (seq_t)0;
        if (a_cap == 0)
            a_cache[0] = (seq_t)0; /* a(0) = 0 */
        a_cap = new_cap;
    }

    /* If result already computed, return it.  */
    if (n <= last_n)
        return a_cache[n];

    /* Ensure the row buffers have enough capacity.  */
    if (row_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        void *tmp;
        tmp = realloc(prev_row, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)
            return (seq_t)0;
        prev_row = (seq_t *)tmp;
        tmp = realloc(cur_row, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)
            return (seq_t)0;
        cur_row = (seq_t *)tmp;
        /* Zero the newly added entries to avoid stray data.  */
        for (size_t i = row_cap; i < new_cap; ++i) {
            prev_row[i] = (seq_t)0;
            cur_row[i]  = (seq_t)0;
        }
        row_cap = new_cap;
    }

    /* Initialise the base Stirling row for n = 0 if this is the first call.  */
    if (last_n == 0) {
        for (size_t i = 0; i < row_cap; ++i)
            prev_row[i] = (seq_t)0;
        prev_row[0] = (seq_t)1;   /* Stirling2(0,0) = 1  */
    }

    /* Compute rows from last_n+1 up to n.  */
    for (idx_t m = last_n + 1; m <= n; ++m) {
        /* Compute Stirling numbers for row m into cur_row.  */
        cur_row[0] = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term2 = prev_row[k - 1];
            seq_t term1 = (k == m) ? (seq_t)0 : ((seq_t)k * prev_row[k]);
            cur_row[k] = term1 + term2;
        }

        /* Compute a(m) = \sum_{k=0}^{m-1} (m)_k * S(m, k+1)  */
        seq_t sum = (seq_t)0;
        seq_t poch = (seq_t)1; /* (m)_0 = 1 */
        for (idx_t k = 0; k < m; ++k) {
            sum += poch * cur_row[k + 1];
            poch *= (m + k);  /* advance Pochhammer */
        }
        a_cache[m] = sum;

        /* Swap rows for the next iteration.  */
        seq_t *tmp = prev_row;
        prev_row = cur_row;
        cur_row  = tmp;
    }

    last_n = n;
    return a_cache[n];
}
