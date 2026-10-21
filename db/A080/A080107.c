/*
 * A080107  Number of fixed points of permutation of SetPartitions under
 *          {1,2,...,n}->{n,n-1,...,1}. Number of symmetric arrangements of
 *          non-attacking rooks on upper half of n X n chessboard.
 * https://oeis.org/A080107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 41, u128 n <= 70,
 *            big any n
 * Helpers:   alloc_buffer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080107: a(n) = Σ_{k=0..n} Ach(n,k)
 * where Ach(0,0)=1, Ach(1,1)=1 and for n≥2
 *      Ach(n,k) = k·Ach(n‑2,k) + Ach(n‑2,k‑1) + Ach(n‑2,k‑2)  (indices <0 give 0)
 * This function memoises computed values and grows the
 * recurrence tables on demand.
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_buffer.h"

/* seq_t and idx_t are provided by the build environment. */
static bool initialized = false;
static idx_t max_n = 0;          /* largest n that has been completely processed */
static seq_t *a_vals = NULL;     /* memoised a(n) values */
static size_t a_cap  = 0;        /* allocated capacity of a_vals */

static seq_t *arr[2] = {NULL, NULL};  /* arrays holding Ach(n,k) for the two parities */
static size_t arr_size[2] = {0, 0};    /* current allocated sizes of the arrays */

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* -----------------------------------------------------------------
     * Initialise static data on first call.
     * ----------------------------------------------------------------- */
    if (!initialized) {
        /* Allocate buffers for parity 0 (even) and parity 1 (odd). */
        arr[0] = alloc_buffer(0, &arr_size[0]);  /* Ach(0,0)=1 */
        if (!arr[0]) return (seq_t)0;
        arr[0][0] = (seq_t)1;

        arr[1] = alloc_buffer(1, &arr_size[1]);  /* Ach(1,1)=1 */
        if (!arr[1]) return (seq_t)0;
        arr[1][0] = (seq_t)0;
        arr[1][1] = (seq_t)1;

        /* Allocate a_vals with room for a(0) and a(1). */
        a_vals = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!a_vals) return (seq_t)0;
        a_vals[0] = (seq_t)1;
        a_vals[1] = (seq_t)1;
        a_cap  = 2;
        max_n = 1;
        initialized = true;
    }

    /* -----------------------------------------------------------------
     * Grow a_vals if necessary.
     * ----------------------------------------------------------------- */
    if ((idx_t)a_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        for (size_t i = a_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        a_vals = tmp;
        a_cap  = new_cap;
    }

    /* -----------------------------------------------------------------
     * If we already have the value, just return it.
     * ----------------------------------------------------------------- */
    if (n <= max_n) {
        return a_vals[n];
    }

    /* -----------------------------------------------------------------
     * Compute successive terms up to n.
     * ----------------------------------------------------------------- */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        unsigned int parity = (unsigned int)(cur & 1U);   /* 0 = even, 1 = odd */
        seq_t *prev_buf = arr[parity];
        size_t prev_len = arr_size[parity];              /* should be (cur-2)+1 */

        /* Allocate a new buffer for Ach(cur,k). */
        seq_t *cur_buf = (seq_t *)malloc((size_t)(cur + 1) * sizeof(seq_t));
        if (!cur_buf) return (seq_t)0;
        for (size_t i = 0; i < (size_t)(cur + 1); ++i) cur_buf[i] = (seq_t)0;

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= cur; ++k) {
            seq_t term1 = (seq_t)0, term2 = (seq_t)0, term3 = (seq_t)0;

            if (k < (idx_t)prev_len)              /* k ≤ (cur-2) */
                term1 = (seq_t)k * prev_buf[k];
            if (k >= 1 && (k - 1) < (idx_t)prev_len)
                term2 = prev_buf[k - 1];
            if (k >= 2 && (k - 2) < (idx_t)prev_len)
                term3 = prev_buf[k - 2];

            cur_buf[k] = term1 + term2 + term3;
            sum += cur_buf[k];
        }

        /* Store the computed a(cur). */
        a_vals[cur] = sum;

        /* Replace the old buffer for this parity with the new one. */
        free(prev_buf);
        arr[parity]   = cur_buf;
        arr_size[parity] = (size_t)(cur + 1);
    }

    max_n = n;
    return a_vals[n];
}
