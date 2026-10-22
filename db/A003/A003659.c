/*
 * A003659  Shifts left under Stirling2 transform.
 * https://oeis.org/A003659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n+1) = Sum_{k=1..n} Stirling2(n,k) * a(k), a(1) = 1. One row
 *            of Stirling2 is kept and extended in place with S(n,k) =
 *            k*S(n-1,k) + S(n-1,k-1), so all terms up to n cost O(n^2)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 29,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *A = NULL;     /* A[1..alen] computed terms */
static seq_t *row = NULL;   /* row[0..row_n] = Stirling2(row_n, k) */
static idx_t alen = 0;
static idx_t row_n = 0;
static idx_t cap = 0;

static void ensure_cap(idx_t need) {
    if (cap >= need) return;
    idx_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    if (cap == 0) {
        A = alloc_zero(nc);
        row = alloc_zero(nc);
    } else {
        A = grow_zero(A, cap, nc);
        row = grow_zero(row, cap, nc);
    }
    cap = nc;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_cap(n + 2);
    if (alen == 0) {
        A[1] = 1;
        alen = 1;
        row[0] = 1;   /* Stirling2(0,0) = 1 */
        row_n = 0;
    }
    while (alen < n) {
        idx_t m = alen;  /* compute a(m+1) using row m */
        while (row_n < m) {
            idx_t r = row_n + 1;
            for (idx_t k = r; k >= 1; k--)
                row[k] = (seq_t)k * row[k] + row[k - 1];
            row[0] = 0;
            row_n = r;
        }
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; k++)
            sum += row[k] * A[k];
        A[m + 1] = sum;
        alen = m + 1;
    }
    return A[n];
}
