/*
 * A073146  Triangle of numbers {a(n,k), n >= 0, 0 <= k <= n} defined by
 *          a(0,0)=1, a(n,0)=A000670(n), a(n,n)=A000629(n), a(n,k) = a(n,k-1)
 *          + a(n-1,k-1); a(n+1,0) = Sum_{k=0..n} a(n,k).
 * https://oeis.org/A073146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A073146 – flattened preferential‑arrangements triangle
 *
 *  Triangle definition (from OEIS):
 *      a(0,0) = 1
 *      a(n,0) = A000670(n)          (preferential arrangements)
 *      a(n,n) = A000629(n)          (necklaces of sets of labeled beads)
 *      a(n,k) = a(n,k-1) + a(n-1,k-1)   for 1 ≤ k ≤ n
 *      a(n+1,0) = Sum_{j=0..n} a(n,j)   (row sum)
 *
 *  The required sequence a(n) is the row‑major flattening of this triangle:
 *      a(0)   = a(0,0)
 *      a(1)   = a(1,0), a(2) = a(1,1), a(3) = a(2,0), …
 *
 *  Implementation:  incremental construction of rows, storing all
 *  already computed flattened values in a dynamic array.  Only the
 *  previous row is kept while the new row is being assembled.
 *
 *  All arithmetic uses type seq_t (unsigned integer or arbitrary
 *  precision), the build supplies it.  The code is C11/C++17 compatible.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool, true, false */
#include <stdlib.h>   /* malloc, free, realloc */

#ifdef __cplusplus
/* In C++ 'new' and 'delete' construct and destroy objects. */
static inline seq_t* seq_alloc(size_t len) { return new seq_t[len]; }
static inline void seq_free(seq_t *p) { delete [] p; }
static inline seq_t* seq_realloc(seq_t *old, size_t old_cap, size_t used, size_t new_cap) {
    seq_t *out = new seq_t[new_cap];
    for (size_t i = 0; i < used; ++i) out[i] = old[i];
    delete [] old;
    return out;
}
#else
/* In C use malloc/free/realloc. */
static inline seq_t* seq_alloc(size_t len) { return (seq_t*)malloc(sizeof(seq_t)*len); }
static inline void seq_free(seq_t *p) { free(p); }
static inline seq_t* seq_realloc(seq_t *old, size_t old_cap, size_t used, size_t new_cap) {
    seq_t *out = (seq_t*)realloc(old, sizeof(seq_t)*new_cap);
    return out;   /* realloc preserves the used data */
}
#endif

/* Return the n-th term of the flattened triangle. */
seq_t a(idx_t n)
{
    /* Static state retained between calls. */
    static bool          init          = false;
    static seq_t*        flat          = NULL;   /* flattened values */
    static size_t        fl_cap        = 0;      /* allocated capacity of flat */
    static size_t        fl_len        = 0;      /* number of values stored */

    static seq_t*        prev_row      = NULL;   /* array holding previous row */
    static size_t        prev_len      = 0;      /* length of previous row */

    static size_t        cur_row       = 0;      /* index of last finished row (0‑based) */
    static seq_t         sum_prev      = 0;      /* a(cur_row+1,0) = sum of previous row */

    /* One‑time initialisation for n = 0. */
    if (!init) {
        fl_cap   = 1;
        flat     = seq_alloc(fl_cap);
        flat[0]  = (seq_t)1;          /* a(0,0) */
        fl_len   = 1;

        prev_len = 1;
        prev_row = seq_alloc(prev_len);
        prev_row[0] = (seq_t)1;       /* row 0 */

        sum_prev = (seq_t)1;          /* sum of row 0 */
        init     = true;
    }

    /* Grow the triangle until we have at least (n+1) flattened entries. */
    while (fl_len <= (size_t)n) {
        /* New row index. */
        size_t m = cur_row + 1;       /* row m will have length m+1 */

        /* Ensure flat array can hold the new row’s entries. */
        if (fl_len + (m + 1) > fl_cap) {
            size_t new_cap = fl_cap ? fl_cap * 2 : 1;
            while (fl_len + (m + 1) > new_cap) new_cap *= 2;
            flat = seq_realloc(flat, fl_cap, fl_len, new_cap);
            fl_cap = new_cap;
        }

        /* Allocate array for the new row. */
        seq_t *new_row = seq_alloc(m + 1);

        /* First element of the new row is the sum of the previous row. */
        new_row[0] = sum_prev;
        flat[fl_len++] = new_row[0];
        seq_t row_sum = new_row[0];

        /* Compute remaining elements of the new row. */
        for (size_t k = 1; k <= m; ++k) {
            new_row[k] = new_row[k - 1] + prev_row[k - 1];
            flat[fl_len++] = new_row[k];
            row_sum += new_row[k];
        }

        /* Replace previous row with the new one. */
        seq_free(prev_row);
        prev_row = new_row;
        prev_len = m + 1;

        /* Update state for next iteration. */
        cur_row  = m;
        sum_prev = row_sum;
    }

    return flat[(size_t)n];
}
