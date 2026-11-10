/*
 * A091497  Number of nonzero terms in the n-th row of triangle A091492.
 * https://oeis.org/A091497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k in 0..n with T(n,k) != 0, where T is A091492:
 *            T(n,0)=1, T(1,1)=1, T(n,k) = Sum_{j=ceil(k/2)..min(k,n-k)}
 *            T(n-k,j)*T(j,k-j). All terms are nonnegative, so only whether
 *            each entry is nonzero is tracked
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static unsigned char **A091497_rows = NULL;
static size_t A091497_cap = 0;
static size_t A091497_cnt = 0;
static size_t *A091497_nz = NULL;

static void A091497_ensure(size_t m)
{
    if (m < A091497_cnt) return;
    if (m + 1 > A091497_cap) {
        size_t nc = (m + 1) * 2;
        unsigned char **r = (unsigned char **)realloc(A091497_rows, nc * sizeof(unsigned char *));
        size_t *z = (size_t *)realloc(A091497_nz, nc * sizeof(size_t));
        A091497_rows = r;
        A091497_nz = z;
        A091497_cap = nc;
    }
    for (size_t n = A091497_cnt; n <= m; ++n) {
        unsigned char *row = (unsigned char *)calloc(n + 1, 1);
        size_t c = 0;
        for (size_t k = 0; k <= n; ++k) {
            unsigned char v = 0;
            if (k == 0) v = 1;
            else if (n == 1 && k == 1) v = 1;
            else {
                size_t start = (k + 1) >> 1;
                size_t end = (n - k < k) ? (n - k) : k;
                for (size_t j = start; j <= end; ++j) {
                    if (A091497_rows[n - k][j] && A091497_rows[j][k - j]) { v = 1; break; }
                }
            }
            row[k] = v;
            c += v;
        }
        A091497_rows[n] = row;
        A091497_nz[n] = c;
    }
    A091497_cnt = m + 1;
}

seq_t a(idx_t n)
{
    A091497_ensure((size_t)n);
    return (seq_t)A091497_nz[(size_t)n];
}
