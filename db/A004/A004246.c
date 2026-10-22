/*
 * A004246  Number of permutations of length n with spread 3.
 * https://oeis.org/A004246
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations p of {0,...,n-1} with
 *            Sum_{i=0..n-1} i*p(i) == 3 (mod n); computed by a dynamic
 *            program over subsets of used values, tracking that sum mod n
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 a4246_cnt;

seq_t a(idx_t n)
{
    static seq_t memo[64];
    static bool have[64];
    if (n < 64 && have[n]) return memo[n];

    int N = (int)n;
    uint32_t full = (uint32_t)1 << N;
    uint32_t *rank = (uint32_t *)malloc((size_t)full * sizeof(uint32_t));
    uint32_t *order = (uint32_t *)malloc((size_t)full * sizeof(uint32_t));
    size_t csz[40], start[40], pos[40];
    int *mul = (int *)malloc((size_t)N * sizeof(int));
    int k, v, r;
    uint32_t m;
    size_t j;
    a4246_cnt *cur, *nxt, res;
    seq_t out;

    if (!rank || !order || !mul) {
        free(rank); free(order); free(mul);
        return (seq_t)0;
    }

    for (k = 0; k <= N; k++) csz[k] = 0;
    for (m = 0; m < full; m++) csz[__builtin_popcount(m)]++;
    start[0] = 0;
    for (k = 1; k <= N; k++) start[k] = start[k - 1] + csz[k - 1];
    for (k = 0; k <= N; k++) pos[k] = start[k];
    for (m = 0; m < full; m++) {
        int pc = __builtin_popcount(m);
        rank[m] = (uint32_t)(pos[pc] - start[pc]);
        order[pos[pc]++] = m;
    }

    cur = (a4246_cnt *)calloc((size_t)N, sizeof(a4246_cnt));
    cur[0] = 1;
    for (k = 0; k < N; k++) {
        nxt = (a4246_cnt *)calloc(csz[k + 1] * (size_t)N, sizeof(a4246_cnt));
        for (v = 0; v < N; v++) mul[v] = (int)(((long long)k * v) % N);
        for (j = 0; j < csz[k]; j++) {
            uint32_t mask = order[start[k] + j];
            a4246_cnt *row = cur + j * (size_t)N;
            bool any = false;
            for (r = 0; r < N; r++) if (row[r]) { any = true; break; }
            if (!any) continue;
            for (v = 0; v < N; v++) {
                if ((mask >> v) & 1u) continue;
                a4246_cnt *dst = nxt + (size_t)rank[mask | ((uint32_t)1 << v)] * (size_t)N;
                int mv = mul[v];
                for (r = 0; r < N; r++) {
                    int t = r + mv;
                    if (t >= N) t -= N;
                    dst[t] += row[r];
                }
            }
        }
        free(cur);
        cur = nxt;
    }

    res = cur[3 % N];
    free(cur);
    free(rank);
    free(order);
    free(mul);

    out = u128_to_seq(res);
    if (n < 64) { memo[n] = out; have[n] = true; }
    return out;
}
