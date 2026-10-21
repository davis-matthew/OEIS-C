/*
 * A065048  Largest unsigned Stirling number of the first kind:
 *          max_k(s(n+1,k)); i.e., largest coefficient of polynomial
 *          x*(x+1)*(x+2)*(x+3)*...*(x+n).
 * https://oeis.org/A065048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1 ≤ k ≤ n+1} |s(n+1,k)| (unsigned Stirling numbers
 *            of the first kind)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static arrays to keep current and previous Stirling rows */
    static seq_t *prev = NULL;     /* s(r-1,0..r-1) */
    static seq_t *cur  = NULL;     /* s(r,0..r)   */
    static size_t cap  = 0;        /* capacity of prev/cur arrays */
    /* cache of answers, a_vals[m] = a(m) */
    static seq_t *a_vals = NULL;
    static size_t a_cap  = 0;      /* capacity of a_vals */
    static idx_t  current_n = 0;   /* last n for which a_vals[n] is valid */

    /* initialise storage on first call */
    if (!prev) {
        cap   = 1;
        prev  = (seq_t*)malloc(cap  * sizeof(seq_t));
        cur   = (seq_t*)malloc(cap  * sizeof(seq_t));
        if (!prev || !cur) return (seq_t)0; /* allocation failure */

        prev[0] = (seq_t)1;           /* s(0,0)=1 */
        a_cap   = 1;
        a_vals  = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (!a_vals) return (seq_t)0;
        current_n = 0;
    }

    /* ensure a_vals can hold answer for n */
    if ((size_t)n + 1 > a_cap) {
        size_t new_a_cap = a_cap ? a_cap * 2 : 1;
        while (new_a_cap < (size_t)n + 1) new_a_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_vals, new_a_cap * sizeof(seq_t));
        if (!tmp) return a_vals[current_n]; /* allocation failure */
        a_vals = tmp;
        a_cap  = new_a_cap;
    }

    /* compute rows up to r = n+1, storing a(current_n) each time */
    while (current_n <= n) {
        idx_t r = current_n + 1;            /* next row index (s(r,•)) */

        /* enlarge prev/cur if required for r+1 entries (0..r) */
        if ((size_t)r + 1 > cap) {
            size_t new_cap = cap ? cap * 2 : 1;
            while (new_cap < (size_t)r + 1) new_cap <<= 1;
            seq_t *tmp1 = (seq_t*)realloc(prev, new_cap * sizeof(seq_t));
            seq_t *tmp2 = (seq_t*)realloc(cur , new_cap * sizeof(seq_t));
            if (!tmp1 || !tmp2) return a_vals[current_n];
            prev = tmp1;
            cur  = tmp2;
            cap = new_cap;
            /* no need to initialise newly added elements – they are never read until used later */
        }

        /* recurrence: s(r,k) = s(r-1,k-1) + (r-1)*s(r-1,k)  for k≥1,  s(r,0)=0 */
        cur[0] = 0;
        seq_t max_val = 0;

        for (idx_t k = 1; k <= r; ++k) {
            seq_t left  = (k-1 <= (idx_t)r-1) ? prev[k-1] : (seq_t)0;
            seq_t right = (k   <= (idx_t)r-1) ? prev[k]   : (seq_t)0;
            cur[k] = left + (seq_t)(r-1) * right;
            if (cur[k] > max_val) max_val = cur[k];
        }

        /* store a(current_n) = max of row r = current_n+1 */
        a_vals[current_n] = max_val;

        /* prepare for next iteration */
        seq_t *tmp = prev; prev = cur; cur = tmp;
        current_n = r;
    }

    return a_vals[n];
}
