/*
 * A003724  Number of partitions of n-set into odd blocks.
 * https://oeis.org/A003724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), where T(n,k) = T(n-2,k-2) +
 *            k^2*T(n-2,k), T(0,0) = 1, T(1,1) = 1, T(1,0) = 0 (number of
 *            partitions of an n-set into k odd blocks)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 28, u128 n <= 46,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

static seq_t *vals = NULL;   /* vals[i] = a(i) for i <= top */
static idx_t vals_cap = 0;
static seq_t *row_prev = NULL; /* row top-1, length top */
static seq_t *row_cur = NULL;  /* row top, length top+1 */
static idx_t top = 0;
static int inited = 0;

static void init_state(void) {
    vals_cap = 16;
    vals = alloc_zero(vals_cap);
    vals[0] = 1;
    vals[1] = 1;
    row_prev = alloc_zero(1);
    row_prev[0] = 1;            /* row 0 */
    row_cur = alloc_zero(2);
    row_cur[1] = 1;             /* row 1 */
    top = 1;
    inited = 1;
}

static void extend_to(idx_t n) {
    while (top < n) {
        idx_t m = top + 1;      /* computing row m from row m-2 = row_prev (length m-1) */
        seq_t *nr = alloc_zero(m + 1);
        seq_t sum = 0;
        idx_t k;
        for (k = 0; k <= m; k++) {
            seq_t v = 0;
            if (k >= 2) v += row_prev[k - 2];
            if (k + 2 <= m) v += (seq_t)(k * k) * row_prev[k];
            nr[k] = v;
            sum += v;
        }
        release(row_prev, m - 1);
        row_prev = row_cur;
        row_cur = nr;
        top = m;
        if (m >= vals_cap) {
            idx_t nc = vals_cap * 2;
            while (nc <= m) nc *= 2;
            vals = grow_zero(vals, vals_cap, nc);
            vals_cap = nc;
        }
        vals[m] = sum;
    }
}

seq_t a(idx_t n) {
    if (!inited) init_state();
    if (n > top) extend_to(n);
    return vals[n];
}
