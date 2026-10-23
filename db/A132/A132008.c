/*
 * A132008  Row sums of triangle A132007.
 * https://oeis.org/A132008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   Triangle T(n,k) with T(0,0)=1,
   T(n,0)=T(n-1,n-1),
   T(n,k)=T(n,k-1)+n*T(n-1,k-1) for k>0.
   Row sums a(n)=∑_{k=0}^{n} T(n,k).               */

#include <stdlib.h>
#include <stddef.h>

extern seq_t a(idx_t n) {
    /* static state for incremental computation */
    static seq_t *a_cache = NULL;  /* array of row sums a(i) */
    static size_t a_cache_len = 0; /* number of stored sums */
    static size_t a_cache_cap = 0; /* current capacity of a_cache */

    static seq_t *prev_row = NULL; /* previous row T(n-1, k) */
    static size_t prev_row_len = 0;/* length of previous row */

    static idx_t last_n = 0;        /* index of the last computed row */

    /* initialization for first call */
    if (a_cache_len == 0) {
        a_cache_cap = 2;
        a_cache = (seq_t*)malloc(a_cache_cap * sizeof(seq_t));
        /* row 0 */
        prev_row_len = 1;
        prev_row = (seq_t*)malloc(sizeof(seq_t));
        prev_row[0] = 1;                /* T(0,0) */
        a_cache[a_cache_len++] = 1;      /* a(0) = 1 */
        last_n = 0;
    }

    /* if requested value already computed, simply return it */
    if (n < a_cache_len) {
        return a_cache[(size_t)n];
    }

    /* compute rows from last_n+1 up to n */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        size_t cur_len = (size_t)i + 1;
        seq_t *curr = (seq_t*)malloc(cur_len * sizeof(seq_t));

        seq_t sum_prev = a_cache[(size_t)last_n];   /* S(n-1) */
        seq_t lastprev = prev_row[prev_row_len - 1];/* T(n-1,n-1) */

        /* k = 0 */
        curr[0] = lastprev;
        seq_t sum_curr = curr[0];

        /* 1 <= k <= i-1 */
        for (idx_t k = 1; k < i; ++k) {
            curr[k] = curr[k - 1] + ((seq_t)i) * prev_row[k - 1];
            sum_curr += curr[k];
        }

        /* k = i */
        curr[i] = lastprev + ((seq_t)i) * sum_prev;
        sum_curr += curr[i];

        /* store the sum in cache, growing if necessary */
        if (a_cache_len == a_cache_cap) {
            a_cache_cap = a_cache_cap ? a_cache_cap * 2 : 2;
            a_cache = (seq_t*)realloc(a_cache, a_cache_cap * sizeof(seq_t));
        }
        a_cache[a_cache_len++] = sum_curr;

        /* discard previous row, keep current for next iteration */
        free(prev_row);
        prev_row = curr;
        prev_row_len = cur_len;
        last_n = i;
    }

    return a_cache[(size_t)n];
}
