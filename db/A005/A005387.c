/*
 * A005387  Number of partitional matroids on n elements.
 * https://oeis.org/A005387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   E.g.f. exp((x-1)*exp(x) + 2*x + 1), differentiated: a(n+1) =
 *            2*a(n) + n * Sum_{j=0..n-1} binomial(n-1, j) * a(n-1-j), a(0) =
 *            1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 39,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    static seq_t *A = NULL;    /* A[k] = a(k) for k < have */
    static seq_t *row = NULL;  /* row[j] = binomial(rowm, j) */
    static idx_t cap = 0;
    static idx_t have = 0;
    static idx_t rowm = 0;

    if (A == NULL) {
        cap = 16;
        A = alloc_zero(cap);
        row = alloc_zero(cap);
        A[0] = (seq_t)1;
        row[0] = (seq_t)1;
        have = 1;
        rowm = 0;
    }

    if (n + 2 > cap) {
        idx_t newcap = 2 * n + 16;
        A = grow_zero(A, cap, newcap);
        row = grow_zero(row, cap, newcap);
        cap = newcap;
    }

    while (have <= n) {
        idx_t i = have - 1;  /* compute a(i+1) */
        seq_t next = (seq_t)2 * A[i];
        if (i > 0) {
            while (rowm < i - 1) {
                rowm++;
                for (idx_t j = rowm; j >= 1; j--)
                    row[j] += row[j - 1];
                row[0] = (seq_t)1;
            }
            seq_t s = (seq_t)0;
            for (idx_t j = 0; j <= i - 1; j++)
                s += row[j] * A[i - 1 - j];
            next += (seq_t)i * s;
        }
        A[i + 1] = next;
        have++;
    }

    return A[n];
}
