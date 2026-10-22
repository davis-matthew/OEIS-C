/*
 * A007558  Shifts 2 places left when e.g.f. is squared.
 * https://oeis.org/A007558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n) = Sum_{k=0..n-2} binomial(n-2,k)*a(k)*a(n-2-k) for n >= 2,
 *            a(0) = a(1) = 1 (from A'' = A^2)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *A7558_c = NULL;   /* computed terms a(0..A7558_have-1) */
static seq_t *A7558_row = NULL; /* Pascal row for m = A7558_rowm */
static idx_t A7558_have = 0;
static idx_t A7558_rowm = 0;

seq_t a(idx_t n) {
    if (A7558_have == 0) {
        A7558_c = alloc_zero(2);
        A7558_c[0] = 1;
        A7558_c[1] = 1;
        A7558_row = alloc_zero(1);
        A7558_row[0] = 1;
        A7558_rowm = 0;
        A7558_have = 2;
    }
    if (n < A7558_have) return A7558_c[n];

    A7558_c = grow_zero(A7558_c, A7558_have, n + 1);
    A7558_row = grow_zero(A7558_row, A7558_rowm + 1, n - 1);

    for (idx_t i = A7558_have; i <= n; i++) {
        idx_t m = i - 2;
        while (A7558_rowm < m) {
            idx_t r = A7558_rowm + 1;
            A7558_row[r] = 1;
            for (idx_t k = r - 1; k >= 1; k--)
                A7558_row[k] += A7558_row[k - 1];
            A7558_rowm = r;
        }
        seq_t sum = 0;
        for (idx_t k = 0; 2 * k <= m; k++) {
            seq_t t = A7558_row[k] * A7558_c[k] * A7558_c[m - k];
            if (2 * k == m) sum += t;
            else sum += t + t;
        }
        A7558_c[i] = sum;
    }
    A7558_have = n + 1;
    return A7558_c[n];
}
