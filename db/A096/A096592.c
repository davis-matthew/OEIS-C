/*
 * A096592  Row sums of triangle A096591, which shifts one place diagonally
 *          left and upward under the matrix square operation.
 * https://oeis.org/A096592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), where T is A096591: T(n,0)=T(n,n)=1,
 *            and for n>1, 0<k<n, T(n,k) = Sum_{i=k-1..n-2} T(n-2,i)*T(i,k-1);
 *            a(0)=1, a(1)=2
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t **A096592_rows = NULL;
static size_t A096592_count = 0;
static size_t A096592_cap = 0;

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    while (A096592_count <= N) {
        size_t m = A096592_count;
        seq_t *cur = mem_new(seq_t, m + 1);
        cur[0] = (seq_t)1;
        cur[m] = (seq_t)1;
        if (m >= 2) {
            for (size_t k = 1; k < m; ++k) {
                seq_t sum = (seq_t)0;
                for (size_t i = k - 1; i + 2 <= m; ++i) {
                    sum += A096592_rows[m - 2][i] * A096592_rows[i][k - 1];
                }
                cur[k] = sum;
            }
        }
        if (m >= A096592_cap) {
            size_t nc = A096592_cap ? A096592_cap * 2 : 16;
            seq_t **tmp = (seq_t **)realloc((void *)A096592_rows, nc * sizeof(seq_t *));
            if (!tmp) abort();
            A096592_rows = tmp;
            A096592_cap = nc;
        }
        A096592_rows[m] = cur;
        A096592_count = m + 1;
    }
    seq_t s = (seq_t)0;
    for (size_t k = 0; k <= N; ++k) s += A096592_rows[N][k];
    return s;
}
