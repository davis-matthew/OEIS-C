/*
 * A005736  Number of degenerate fanout-free Boolean functions of n variables
 *          using And, Or and Not gates.
 * https://oeis.org/A005736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k)*s(k), where s(0) = 2, s(1) =
 *            2, s(k) = A005640(k+1) = 2^(k+1)*A000311(k) for k >= 2; A000311
 *            from the EGF identity A'(x)*(1 + x - 2*A(x)) = 1: A000311(m+1) =
 *            2*Sum_{k=1..m} binomial(m,k)*A000311(k)*A000311(m-k+1) -
 *            m*A000311(m), A000311(0) = 0, A000311(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

static seq_t *a5736_S = NULL;   /* a5736_S[k] = A000311(k) */
static idx_t a5736_cnt = 0;

static void a5736_ensure(idx_t N)
{
    idx_t m, k;
    seq_t *row;
    if (a5736_cnt > N) return;
    if (a5736_S == NULL) {
        a5736_S = alloc_zero(N + 1);
    } else {
        a5736_S = grow_zero(a5736_S, a5736_cnt, N + 1);
    }
    a5736_S[0] = 0;
    if (N >= 1) a5736_S[1] = 1;
    row = alloc_zero(N + 2);
    row[0] = 1;
    for (m = 0; m < N; m++) {
        /* row holds binomial(m, .) */
        if (m + 1 >= a5736_cnt && m + 1 >= 2) {
            seq_t sum = 0;
            seq_t sub;
            for (k = 1; k <= m; k++)
                sum += row[k] * a5736_S[k] * a5736_S[m - k + 1];
            sub = (seq_t)m * a5736_S[m];
            a5736_S[m + 1] = sum * 2 - sub;
        }
        for (k = m + 1; k >= 1; k--)
            row[k] += row[k - 1];
    }
    release(row, N + 2);
    a5736_cnt = N + 1;
}

seq_t a(idx_t n)
{
    idx_t m, k;
    seq_t *row;
    seq_t total = 0;
    seq_t p;
    if (n == 0) return 0;
    a5736_ensure(n);
    row = alloc_zero(n + 1);
    row[0] = 1;
    for (m = 0; m < n; m++)
        for (k = m + 1; k >= 1; k--)
            row[k] += row[k - 1];
    /* row holds binomial(n, .) */
    total += row[0] * 2;
    if (n >= 2) total += row[1] * 2;
    p = 4;
    for (k = 2; k < n; k++) {
        p *= 2;  /* p = 2^(k+1) */
        total += row[k] * p * a5736_S[k];
    }
    release(row, n + 1);
    return total;
}
