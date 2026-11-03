/*
 * A007603  Power-sum numbers: let n = a_1 a_2 ... a_k be a k-digit number; n
 *          is a power-sum number if there are exponents e_1 ... e_m such that
 *          n = Sum_{i=1..m} Sum_{j=1..k} a_j^e_i.
 * https://oeis.org/A007603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   search over n: n is a term iff n is a nonnegative integer
 *            combination of the power sums S_e = Sum_j a_j^e (e >= 1) of its
 *            digits; tested by a shortest-path over residues mod S_1 (the
 *            digit sum)
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9808 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool a7603_test(long long m) {
    int dig[24];
    int nd = 0, maxd = 0;
    long long s1 = 0, t = m;
    while (t > 0) {
        dig[nd] = (int)(t % 10);
        s1 += dig[nd];
        if (dig[nd] > maxd) maxd = dig[nd];
        nd++;
        t /= 10;
    }
    if (s1 == 0) return false;
    if (m % s1 == 0) return true;
    if (maxd <= 1) return false;

    long long coins[64];
    int nc = 0;
    long long pw[24];
    for (int j = 0; j < nd; j++) pw[j] = dig[j];
    for (;;) {
        long long s = 0;
        for (int j = 0; j < nd; j++) {
            pw[j] *= dig[j];
            s += pw[j];
        }
        if (s > m) break;
        coins[nc++] = s;
    }
    if (nc == 0) return false;

    /* Dijkstra over residues mod s1 */
    long long dist[256];
    bool done[256];
    const long long INF = (long long)1 << 60;
    for (long long r = 0; r < s1; r++) { dist[r] = INF; done[r] = false; }
    dist[0] = 0;
    for (;;) {
        long long best = -1;
        for (long long r = 0; r < s1; r++)
            if (!done[r] && dist[r] < INF && (best < 0 || dist[r] < dist[best])) best = r;
        if (best < 0) break;
        done[best] = true;
        if (dist[best] > m) break;
        for (int c = 0; c < nc; c++) {
            long long nv = dist[best] + coins[c];
            long long nr = nv % s1;
            if (nv < dist[nr]) dist[nr] = nv;
        }
    }
    return dist[m % s1] <= m;
}

static long long *a7603_cache = NULL;
static idx_t a7603_cnt = 0, a7603_cap = 0;
static long long a7603_last = 0;

seq_t a(idx_t n) {
    while (a7603_cnt < n) {
        a7603_last++;
        if (a7603_test(a7603_last)) {
            if (a7603_cnt == a7603_cap) {
                idx_t nc = a7603_cap ? a7603_cap * 2 : 1024;
                a7603_cache = (long long *)realloc(a7603_cache, (size_t)nc * sizeof(long long));
                a7603_cap = nc;
            }
            a7603_cache[a7603_cnt++] = a7603_last;
        }
    }
    seq_t r = (seq_t)(uint64_t)a7603_cache[n - 1];
    return r;
}
