/*
 * A202527  Number of (n+2) X 5 binary arrays avoiding patterns 000 and 101 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 21, u64 n <= 50, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a
   Sequence: A202527
   Formula: a(1..8) = 417,1084,2263,4669,10240,22118,47098,101249,
            a(n) = 2*a(n-1)+2*a(n-3)-2*a(n-4)-2*a(n-5)+a(n-7)  (n>8)
   Note: n is 1‑based.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---- Memoisation array ------------------------------------------------- */
    static seq_t *cache   = NULL; /* array of computed terms              */
    static idx_t cache_cap = 0;   /* allocated capacity (in terms)         */
    static idx_t cache_len = 0;   /* number of terms already initialised   */

    /* ---- Initial values ---------------------------------------------------- */
    enum { init_len = 8 }; /* compile‑time constant */
    static const seq_t init_vals[init_len] = {
        (seq_t)417,  (seq_t)1084, (seq_t)2263, (seq_t)4669,
        (seq_t)10240, (seq_t)22118, (seq_t)47098, (seq_t)101249
    };

    /* ---- Grow the cache if required ---------------------------------------- */
    if (n > cache_cap) {
        size_t new_cap = (size_t)n * 2;
        if (new_cap < 16) new_cap = 16;  /* avoid tiny allocations          */
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) {
            /* Out of memory: terminate program (no handle required).     */
            exit(EXIT_FAILURE);
        }
        cache = newp;
        cache_cap = (idx_t)new_cap;
    }

    /* ---- Initialise first terms once -------------------------------------- */
    if (cache_len == 0) {
        for (idx_t i = 0; i < init_len; ++i)
            cache[i] = init_vals[i];
        cache_len = init_len;
    }

    /* ---- Compute terms up to n (inclusive) --------------------------------- */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i corresponds to a(i+1).  Recurrence applies for i >= 8. */
        if (i >= 8) {
            seq_t val = 0;
            val += (seq_t)2 * cache[i - 1]          /* 2*a(n-1) */
                 + (seq_t)2 * cache[i - 3]          /* 2*a(n-3) */
                 - (seq_t)2 * cache[i - 4]          /* -2*a(n-4) */
                 - (seq_t)2 * cache[i - 5]          /* -2*a(n-5) */
                 +      cache[i - 7];              /* + a(n-7)  */
            cache[i] = val;
        } else {
            /* For i < 8 the value is already set by init_vals. */
        }
    }

    cache_len = n;           /* all terms up to n are now valid         */
    return cache[n - 1];     /* 1‑based indexing                         */
}
