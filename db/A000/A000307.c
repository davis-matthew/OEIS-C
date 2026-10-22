/*
 * A000307  Number of 4-level labeled rooted trees with n leaves.
 * https://oeis.org/A000307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   E.g.f. exp(exp(exp(exp(x)-1)-1)-1), computed as three Stirling
 *            transforms of the all-ones sequence: Bell(n) = Sum_{k=0..n}
 *            Stirling2(n,k) * 1, A000258(n) = Sum_{k=0..n} Stirling2(n,k) *
 *            Bell(k), a(n) = Sum_{k=0..n} Stirling2(n,k) * A000258(k). Each
 *            transform b -> c is evaluated as c(j) = (L^j b)(...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   a307_stirling, zalloc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/zalloc_s.h"
#include "../utils/a307_stirling.h"

static seq_t *a307_res = 0;
static idx_t a307_cap = 0;

seq_t a(idx_t n)
{
    if (n >= a307_cap) {
        idx_t N = n, k;
        seq_t *A, *B, *W;
        if (N < 64)
            N = 64;
        A = zalloc_s(N + 1);
        B = zalloc_s(N + 1);
        W = zalloc_s(N + 1);
        for (k = 0; k <= N; k++)
            A[k] = 1;
        a307_stirling(A, B, W, N);   /* Bell numbers */
        a307_stirling(B, A, W, N);   /* A000258 */
        a307_stirling(A, B, W, N);   /* A000307 */
        a307_res = B;
        a307_cap = N + 1;
    }
    return a307_res[n];
}
