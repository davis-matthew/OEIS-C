/*
 * A092918  Number of connected hypergraphs (without multiple edges and
 *          without empty edges) on n labeled vertices.
 * https://oeis.org/A092918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. log(Sum_{n>=0} 2^(2^n-1) x^n/n!) (with the n=0 term 1);
 *            i.e. a(n) = b(n) - Sum_{k=1..n-1} binomial(n-1,k-1)*a(k)*b(n-k),
 *            b(0)=1, b(n)=2^(2^n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *A = mem_new(seq_t, N + 2);
    seq_t *B = mem_new(seq_t, N + 2);
    seq_t *row = mem_new(seq_t, N + 2);
    B[0] = 1;
    for (size_t m = 1; m <= N; m++) {
        B[m] = 2 * B[m - 1] * B[m - 1];
    }
    row[0] = 1;
    seq_t result = 0;
    for (size_t m = 1; m <= N; m++) {
        /* row[j] = C(m-1, j) */
        seq_t s = 0;
        for (size_t k = 1; k < m; k++) {
            s += row[k - 1] * A[k] * B[m - k];
        }
        A[m] = B[m] - s;
        for (size_t j = m; j >= 1; j--) {
            row[j] = row[j] + row[j - 1];
        }
    }
    result = A[N];
    mem_free(A);
    mem_free(B);
    mem_free(row);
    return result;
}
