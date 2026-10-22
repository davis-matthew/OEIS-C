/*
 * A003575  Dowling numbers: e.g.f.: exp(x + (exp(b*x) - 1)/b) with b=3.
 * https://oeis.org/A003575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} W(n,k), where W(n,k) are the Whitney numbers
 *            of the second kind for the Dowling lattice with b=3: W(n,k) =
 *            (3*k+1)*W(n-1,k) + W(n-1,k-1), W(0,0) = 1, W(n,k) = 0 for k<0 or
 *            k>n. The table is extended row by row and memoized
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"

static seq_t *a003575_row = NULL;
static seq_t *a003575_vals = NULL;
static idx_t a003575_cap = 0;
static idx_t a003575_done = 0;

seq_t a(idx_t n) {
    if (n + 1 > a003575_cap) {
        idx_t newcap = a003575_cap * 2;
        if (newcap < n + 1) newcap = n + 1;
        if (newcap < 16) newcap = 16;
        a003575_row = A_grow(a003575_row, a003575_cap, newcap);
        a003575_vals = A_grow(a003575_vals, a003575_cap, newcap);
        a003575_cap = newcap;
    }
    if (a003575_done == 0) {
        a003575_row[0] = 1;
        a003575_vals[0] = 1;
        a003575_done = 1;
    }
    for (idx_t m = a003575_done; m <= n; m++) {
        seq_t *row = a003575_row;
        row[m] = 0;
        seq_t sum = 1; /* W(m,0) = 1 */
        for (idx_t k = m; k >= 1; k--) {
            row[k] = row[k] * (seq_t)(3 * k + 1) + row[k - 1];
            sum += row[k];
        }
        a003575_vals[m] = sum;
        a003575_done = m + 1;
    }
    return a003575_vals[n];
}
