/*
 * A118103  First differences contain a(n) n's, in order, starting with
 *          a(0)=1.
 * https://oeis.org/A118103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 1
 *   For n > 0: a(n) = a(n-1) + d_n,
 *   where d_n is the nth difference in an infinite sequence that
 *   contains each integer k exactly a(k) times, in increasing order.
 *   The first difference is 0 repeated a(0) times, the next 1 repeated
 *   a(1) times, etc.  This reproduces the OEIS sequence A118103.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"

static seq_t *A = NULL;          /* cache of a(n) values */
static idx_t A_len = 0;          /* number of elements stored */
static idx_t A_cap = 0;          /* allocated capacity of A */

static idx_t cur_diff = 0;       /* current difference value (k) */
static seq_t rem = 0;            /* remaining repetitions of cur_diff */

/* Ensure that A has room for indices < need.  The old buffer is
 * preserved but not freed (to avoid a double‑free bug). */
static void ensure_capacity(idx_t need)
{
    if (need <= A_cap)
        return;
    idx_t new_cap = A_cap ? (A_cap << 1) : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = seq_alloc(new_cap);
    for (idx_t i = 0; i < A_len; ++i)
        new_arr[i] = A[i];
    A = new_arr;
    A_cap = new_cap;
}

/* Return the nth term of A118103. */
seq_t a(idx_t n)
{
    /* Initialise on first call. */
    if (A_len == 0) {
        ensure_capacity(1);
        A[0] = (seq_t)1;        /* a(0) = 1 */
        A_len = 1;
        cur_diff = 0;
        rem = A[0];             /* block length for difference 0 */
    }

    /* If the value is already computed, return it. */
    if (n < A_len)
        return A[n];

    ensure_capacity(n + 1);

    /* Expand the cache up to n. */
    for (idx_t idx = A_len; idx <= n; ++idx) {
        /* Use current difference. */
        seq_t diff = cur_diff;
        A[idx] = A[idx - 1] + diff;

        /* Consume one occurrence of this difference. */
        rem--;
        if (rem == 0) {
            /* Advance to the next difference value. */
            cur_diff++;
            /* The block length for the new difference is a[cur_diff]. */
            /* Since cur_diff <= idx, a[cur_diff] is already available. */
            rem = A[cur_diff];
        }
    }

    A_len = n + 1;
    return A[n];
}
