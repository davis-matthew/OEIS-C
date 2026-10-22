/*
 * A007627  Primitive modest numbers.
 * https://oeis.org/A007627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 324, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A7627_cmp, A7627_gcd, A7627_push
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: n = a*10^k + b with b | (10^k - 1), 1 <= a < b, gcd(a,b) = 1
   (equivalent to n == a (mod b), a < b < 10^k). For a bound L = 10^m, every
   such n < L is generated for k = 1..m-1; the list is sorted and deduplicated,
   and m grows until it holds at least n terms. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/A7627_cmp.h"
#include "../utils/A7627_gcd.h"
#include "../utils/A7627_push.h"

static uint64_t *A7627_list = NULL;
static size_t A7627_cnt = 0;
static int A7627_m = 1;

static void A7627_build(int m) {
    uint64_t L = 1;
    int i, k;
    for (i = 0; i < m; i++) L *= 10;
    uint64_t *buf = NULL;
    size_t len = 0, cap = 0;
    uint64_t P = 1;
    for (k = 1; k < m; k++) {
        P *= 10;
        uint64_t N = P - 1;
        uint64_t amax_lim = (L - 1) / P; /* a*P + b < L requires a <= this */
        if (amax_lim == 0) break;
        uint64_t d;
        for (d = 1; d * d <= N; d++) {
            if (N % d) continue;
            uint64_t bs[2];
            int nb = 0;
            bs[nb++] = d;
            if (d != N / d) bs[nb++] = N / d;
            int j;
            for (j = 0; j < nb; j++) {
                uint64_t b = bs[j];
                if (b < 2) continue;
                uint64_t amax = b - 1;
                if (amax > amax_lim) amax = amax_lim;
                uint64_t aa;
                for (aa = 1; aa <= amax; aa++) {
                    if (A7627_gcd(aa, b) != 1) continue;
                    uint64_t v = aa * P + b;
                    if (v < L) A7627_push(&buf, &len, &cap, v);
                }
            }
        }
    }
    if (len) qsort(buf, len, sizeof(uint64_t), A7627_cmp);
    size_t w = 0, r;
    for (r = 0; r < len; r++)
        if (w == 0 || buf[w - 1] != buf[r]) buf[w++] = buf[r];
    free(A7627_list);
    A7627_list = buf;
    A7627_cnt = w;
    A7627_m = m;
}

seq_t a(idx_t n) {
    while ((idx_t)A7627_cnt < n && A7627_m < 19)
        A7627_build(A7627_m + 1);
    if ((idx_t)A7627_cnt < n) return (seq_t)0;
    return (seq_t)A7627_list[n - 1];
}
