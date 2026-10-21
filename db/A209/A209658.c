/*
 * A209658  Partition numbers p(n) having the same parity as n.
 * https://oeis.org/A209658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = partition number p(m) where p(m) has the same parity as m.
   Partition numbers are generated with Euler's pentagonal recurrence.
   Two accumulators (pos, neg) avoid signed arithmetic: the final
   p(m) = pos - neg and for those m where (p(m) & 1) == (m & 1) we store
   p(m) in the output list.  The routine caches all intermediate values,
   so any call order is O(1). */

#include <stdlib.h>

static seq_t *p_vals = NULL;    /* p[0]..p[current] */
static size_t p_cap  = 0;
static idx_t  p_len  = 0;        /* next free index in p_vals */

static seq_t *sel_vals = NULL;   /* selected partition numbers */
static size_t sel_cap  = 0;
static idx_t  sel_len  = 0;      /* number of selected terms stored */

/* Ensure the array has space for at least 'needed' elements. */
static void grow_array(void **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        *cap = (*cap) ? (*cap << 1) : 8;
        *arr = realloc(*arr, *cap * sizeof(seq_t));
        if (!*arr) exit(1);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Initialise the partition table with p(0) = 1 if it hasn't been done. */
    if (p_len == 0) {
        grow_array((void**)&p_vals, &p_cap, 1);
        p_vals[0] = (seq_t)1;
        p_len = 1;
    }

    /* Compute until we have at least n selected terms. */
    while (sel_len < n) {
        idx_t i = p_len;                       /* next index to compute */

        /* Ensure space for this new p[i]. */
        grow_array((void**)&p_vals, &p_cap, i + 1);

        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1) pos += p_vals[i - pent1];
            else       neg += p_vals[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if (k & 1) pos += p_vals[i - pent2];
            else       neg += p_vals[i - pent2];
        }

        seq_t p_i = pos - neg;              /* pos >= neg by construction */
        p_vals[p_len++] = p_i;

        /* Store the term if its parity matches its index. */
        if (((p_i & (seq_t)1) == (seq_t)(i & 1))) {
            grow_array((void**)&sel_vals, &sel_cap, sel_len + 1);
            sel_vals[sel_len++] = p_i;
        }
    }

    return sel_vals[n - 1];   /* offset 1 */
}
