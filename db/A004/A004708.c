/*
 * A004708  Expansion of 1/(11 - Sum_{k=1..10} exp(k*x)).
 * https://oeis.org/A004708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. A(x) = 1/(11 - Sum_{k=1..10} exp(k*x)), so a(n) =
 *            Sum_{j=0..n-1} binomial(n,j) * a(j) * H(n-j), a(0) = 1, where
 *            H(m) = Sum_{k=1..10} k^m
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    static seq_t *A = NULL;   /* sequence terms */
    static seq_t *H = NULL;   /* power sums H(m) */
    static seq_t *row = NULL; /* Pascal row for m = filled-1 */
    static seq_t pw[11];
    static idx_t cap = 0;
    static idx_t filled = 0;

    if (cap < n + 1) {
        idx_t newcap = n + 1;
        if (newcap < 2 * cap) newcap = 2 * cap;
        if (cap == 0) {
            A = alloc_zero(newcap);
            H = alloc_zero(newcap);
            row = alloc_zero(newcap);
        } else {
            A = grow_zero(A, cap, newcap);
            H = grow_zero(H, cap, newcap);
            row = grow_zero(row, cap, newcap);
        }
        cap = newcap;
    }

    if (filled == 0) {
        int i;
        A[0] = 1;
        row[0] = 1;
        for (i = 1; i <= 10; i++) pw[i] = 1;
        filled = 1;
    }

    for (idx_t m = filled; m <= n; m++) {
        int i;
        idx_t j;
        seq_t s;

        /* advance Pascal row from m-1 to m */
        row[m] = 0;
        for (j = m; j >= 1; j--) row[j] = row[j] + row[j - 1];

        /* H(m) = Sum_{k=1..10} k^m */
        s = 0;
        for (i = 1; i <= 10; i++) {
            pw[i] = pw[i] * (seq_t)i;
            s = s + pw[i];
        }
        H[m] = s;

        s = 0;
        for (j = 0; j < m; j++) {
            s = s + row[j] * A[j] * H[m - j];
        }
        A[m] = s;
        filled = m + 1;
    }

    return A[n];
}
