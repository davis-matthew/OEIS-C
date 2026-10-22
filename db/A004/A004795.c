/*
 * A004795  Least positive unitary linear combination of distinct numbers in
 *          row n of Pascal's triangle; i.e., least positive sum of form
 *          d(0)C(n-1,0) + d(1)C(n-1,1) + ...+ d(m)C(n-1,m), d(i)=+-1, m =
 *          floor((n+1)/2).
 * https://oeis.org/A004795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive value of |sum_{j=0..floor((n-1)/2)}
 *            d(j)*C(n-1,j)|, d(j)=+-1 (the distinct numbers in row n-1);
 *            computed by branch-and-bound search over the signs
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 *g_v;
static u128 *g_suf;
static idx_t g_k;
static u128 g_best;
static u128 g_par;

static void dfs(idx_t i, u128 r) {
    if (g_best == g_par) return;
    if (i == g_k) {
        if (r > 0 && r < g_best) g_best = r;
        return;
    }
    if (r > g_suf[i] && r - g_suf[i] >= g_best) return;
    u128 c = g_v[i];
    u128 d = (r >= c) ? r - c : c - r;
    dfs(i + 1, d);
    dfs(i + 1, r + c);
}

seq_t a(idx_t n) {
    idx_t row = n - 1;
    idx_t mid = row / 2;
    idx_t k = mid + 1;
    idx_t j;
    u128 *bin = (u128 *)malloc((size_t)(k + 1) * sizeof(u128));
    u128 *v = (u128 *)malloc((size_t)(k + 1) * sizeof(u128));
    u128 *suf = (u128 *)malloc((size_t)(k + 1) * sizeof(u128));
    bin[0] = 1;
    for (j = 1; j <= mid; j++)
        bin[j] = bin[j - 1] * (u128)(row - j + 1) / (u128)j;
    /* descending order: C(row,mid), ..., C(row,0) */
    for (j = 0; j < k; j++) v[j] = bin[mid - j];
    suf[k] = 0;
    for (j = k; j-- > 0;) suf[j] = suf[j + 1] + v[j];
    g_v = v;
    g_suf = suf;
    g_k = k;
    g_best = suf[0];
    g_par = (suf[0] & 1) ? 1 : 2;
    dfs(1, v[0]);
    u128 ans = g_best;
    free(bin);
    free(v);
    free(suf);
    return u128_to_seq(ans);
}
