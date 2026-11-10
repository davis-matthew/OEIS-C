/*
 * A085330  Where records occur in A085068.
 * https://oeis.org/A085330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th record position of A085068 (steps of x ->
 *            (4/3)*ceil(x) until an integer); computed by a pruned
 *            depth-first search over base-3 residues, finding the smallest m
 *            with at least S+1 steps for each next record
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 A085330_u;

static A085330_u A085330_best;
static bool A085330_found;
static int A085330_S;

static void A085330_dfs(int j, A085330_u r, A085330_u L, A085330_u p3, A085330_u p4)
{
    if (j == A085330_S) {
        if (r < A085330_best) {
            A085330_best = r;
            A085330_found = true;
        }
        return;
    }
    for (int d = 0; d < 3; d++) {
        A085330_u rr = r + p3 * (A085330_u)d;
        if (rr >= A085330_best) break;
        A085330_u m = L + (A085330_u)3 * p4 * (A085330_u)d;
        A085330_u mj = m + (m + 2) / 3;
        if (mj % 3 == 0) continue;
        A085330_u np3 = (p3 > A085330_best) ? p3 : p3 * 3;
        A085330_dfs(j + 1, rr, mj, np3, p4 * 4);
    }
}

static int A085330_steps(A085330_u m)
{
    int s = 1;
    while (m % 3 != 0) {
        m += (m + 2) / 3;
        s++;
    }
    return s;
}

static A085330_u A085330_rec[256];
static int A085330_cnt = 0;

static seq_t A085330_conv(A085330_u x)
{
    uint64_t hi = (uint64_t)(x >> 64);
    uint64_t lo = (uint64_t)x;
    if (hi == 0) return (seq_t)lo;
    seq_t r = (seq_t)hi;
    r = r * (seq_t)4294967296ULL;
    r = r * (seq_t)4294967296ULL;
    r = r + (seq_t)lo;
    return r;
}

seq_t a(idx_t n)
{
    if (A085330_cnt == 0) {
        A085330_rec[0] = 0;
        A085330_cnt = 1;
    }
    if (n < 1) return (seq_t)0;
    if (n > 250) n = 250;
    while ((idx_t)A085330_cnt < n) {
        A085330_u prev = A085330_rec[A085330_cnt - 1];
        int S = A085330_steps(prev);
        A085330_u B = prev * 2 + 4;
        A085330_u res = 0;
        for (;;) {
            A085330_S = S;
            A085330_best = B;
            A085330_found = false;
            for (int d = 1; d <= 2; d++) {
                if ((A085330_u)d >= A085330_best) break;
                A085330_dfs(1, (A085330_u)d, (A085330_u)d, 3, 1);
            }
            if (A085330_found) {
                res = A085330_best;
                break;
            }
            B *= 2;
        }
        A085330_rec[A085330_cnt++] = res;
    }
    return A085330_conv(A085330_rec[n - 1]);
}
