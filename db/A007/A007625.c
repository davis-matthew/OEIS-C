/*
 * A007625  Number of M-sequences from multicomplexes on at most 6 variables
 *          with no monomial of degree more than n-1.
 * https://oeis.org/A007625
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(-1) = 1; for n >= 0, a(n) = 1 + (number of M-sequences
 *            (1,h_1,...,h_n) with h_1 <= 6), where each step must satisfy
 *            Macaulay's bound h_{i+1} <= h_i^<i>. Computed by dynamic
 *            programming over the value of h_i, using prefix sums, from
 *            degree n down to degree 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   alloc_zero, cb, mac_up, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/cb.h"
#include "../utils/mac_up.h"

#define MEMO_N 128
static seq_t memo[MEMO_N];
static bool memo_ok[MEMO_N];

seq_t a(idx_t n)
{
    seq_t *P, *Q, res;
    long long M, Mi, v, i;
    if (n < 0) return (seq_t)1;
    if (n == 0) return (seq_t)2;
    if (n < MEMO_N && memo_ok[n]) return memo[n];

    M = (long long)cb(n, 5);
    P = alloc_zero((idx_t)(M + 1));
    for (v = 0; v <= M; v++)
        P[v] = (seq_t)(idx_t)v + 1;
    for (i = n - 1; i >= 1; i--) {
        seq_t acc = 0;
        Mi = (long long)cb(i, 5);
        Q = alloc_zero((idx_t)(Mi + 1));
        for (v = 0; v <= Mi; v++) {
            acc += P[(long long)mac_up((unsigned long long)v, i)];
            Q[v] = acc;
        }
        release(P, (idx_t)(M + 1));
        P = Q;
        M = Mi;
    }
    res = P[6] + 1;
    release(P, (idx_t)(M + 1));
    if (n < MEMO_N) { memo[n] = res; memo_ok[n] = true; }
    return res;
}
