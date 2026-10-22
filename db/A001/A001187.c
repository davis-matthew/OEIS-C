/*
 * A001187  Number of connected labeled graphs with n nodes.
 * https://oeis.org/A001187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^binomial(n,2) - Sum_{k=1..n-1} binomial(n-1,k-1) * a(k)
 *            * 2^binomial(n-k,2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   A001187_shl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/A001187_shl.h"

#define A001187_MAXN 10002

static seq_t A001187_A[A001187_MAXN];
static seq_t A001187_P[A001187_MAXN];
static seq_t A001187_R[A001187_MAXN];
static idx_t A001187_cnt = 0;
static idx_t A001187_rowr = 0;

seq_t a(idx_t n) {
    if (n >= A001187_MAXN)
        return 0;
    while (A001187_cnt <= n) {
        idx_t i = A001187_cnt;
        if (i == 0) {
            A001187_A[0] = 1;
            A001187_P[0] = 1;
            A001187_R[0] = 1;
            A001187_rowr = 0;
            A001187_cnt = 1;
            continue;
        }
        if (i == 1)
            A001187_P[1] = 1;
        else
            A001187_P[i] = A001187_shl(A001187_P[i - 1], i - 1);
        while (A001187_rowr < i - 1) {
            idx_t j;
            A001187_R[A001187_rowr + 1] = 1;
            for (j = A001187_rowr; j >= 1; j--)
                A001187_R[j] += A001187_R[j - 1];
            A001187_rowr++;
        }
        {
            seq_t sum = 0;
            idx_t k;
            for (k = 1; k < i; k++)
                sum += A001187_R[k - 1] * A001187_A[k] * A001187_P[i - k];
            A001187_A[i] = A001187_P[i] - sum;
        }
        A001187_cnt++;
    }
    return A001187_A[n];
}
