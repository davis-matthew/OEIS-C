/*
 * A003389  Numbers that are the sum of 11 positive 8th powers.
 * https://oeis.org/A003389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all multisets of 11 positive 8th powers with
 *            sum <= X, sort and deduplicate, doubling X until at least n
 *            distinct sums are found
 * Range:     u8 n <= 1, u16 n <= 75, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A3389_cmpu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/A3389_cmpu.h"

static uint64_t *A3389_vals = NULL;
static size_t A3389_cnt = 0, A3389_cap = 0;
static uint64_t A3389_p8[256];
static int A3389_maxb = 0;

static void A3389_push(uint64_t v) {
    if (A3389_cnt == A3389_cap) {
        size_t nc = A3389_cap ? 2 * A3389_cap : 1024;
        A3389_vals = (uint64_t *)realloc(A3389_vals, nc * sizeof(uint64_t));
        A3389_cap = nc;
    }
    A3389_vals[A3389_cnt++] = v;
}

static void A3389_rec(int rem, int minb, uint64_t sum, uint64_t X) {
    int b;
    if (rem == 0) { A3389_push(sum); return; }
    for (b = minb; b <= A3389_maxb; b++) {
        if (A3389_p8[b] > (X - sum) / (uint64_t)rem) break;
        A3389_rec(rem - 1, b, sum + A3389_p8[b], X);
    }
}

seq_t a(idx_t n) {
    static uint64_t *uniq = NULL;
    static size_t ucnt = 0;
    static uint64_t X = 0;
    while (uniq == NULL || ucnt < n) {
        uint64_t b;
        size_t i, k;
        X = X ? X * 2 : ((uint64_t)1 << 16);
        A3389_maxb = 0;
        for (b = 1; b < 255; b++) {
            uint64_t p = 1;
            int j, ok = 1;
            for (j = 0; j < 8; j++) {
                if (p > X / b) { ok = 0; break; }
                p *= b;
            }
            if (!ok || p > X) break;
            A3389_p8[b] = p;
            A3389_maxb = (int)b;
        }
        A3389_cnt = 0;
        A3389_rec(11, 1, 0, X);
        qsort(A3389_vals, A3389_cnt, sizeof(uint64_t), A3389_cmpu);
        k = 0;
        for (i = 0; i < A3389_cnt; i++)
            if (k == 0 || A3389_vals[i] != A3389_vals[k - 1])
                A3389_vals[k++] = A3389_vals[i];
        free(uniq);
        uniq = (uint64_t *)malloc((k ? k : 1) * sizeof(uint64_t));
        for (i = 0; i < k; i++) uniq[i] = A3389_vals[i];
        ucnt = k;
    }
    return (seq_t)uniq[n - 1];
}
