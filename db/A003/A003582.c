/*
 * A003582  Dowling numbers: e.g.f. exp(x + (exp(b*x)-1)/b) with b=10.
 * https://oeis.org/A003582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} W(n,k), where W(n,k) = W(n-1,k-1) +
 *            (10*k+1)*W(n-1,k), W(0,0) = 1 (r-Whitney numbers of the Dowling
 *            lattice)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"

static seq_t *W_row = NULL;   /* current row W(N-1, 0..N-1), capacity cap+1 */
static seq_t *A_val = NULL;   /* computed terms a(0..N-1), capacity cap */
static idx_t N_done = 0;
static idx_t cap_done = 0;

seq_t a(idx_t n) {
    if (cap_done == 0) {
        cap_done = 64;
        W_row = A_grow(NULL, 0, cap_done + 1);
        A_val = A_grow(NULL, 0, cap_done);
        W_row[0] = 1;
        A_val[0] = 1;
        N_done = 1;
    }
    while (N_done <= n) {
        if (N_done >= cap_done) {
            idx_t newcap = cap_done * 2;
            if (newcap < n + 1) newcap = n + 1;
            W_row = A_grow(W_row, cap_done + 1, newcap + 1);
            A_val = A_grow(A_val, cap_done, newcap);
            cap_done = newcap;
        }
        idx_t m = N_done;
        W_row[m] = 1;
        seq_t sum = 2;  /* W(m,m) + W(m,0) */
        for (idx_t k = m - 1; k >= 1; k--) {
            W_row[k] = W_row[k - 1] + (seq_t)(10 * k + 1) * W_row[k];
            sum += W_row[k];
        }
        W_row[0] = 1;
        A_val[m] = sum;
        N_done++;
    }
    return A_val[n];
}
