/*
 * A207128  Number of n X n 0..3 arrays avoiding the pattern z z+1 z in any
 *          row or column.
 * https://oeis.org/A207128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by a transfer-matrix search: fill the n X n grid cell
 *            by cell, tracking for each column (and the current row) the last
 *            value and whether the last two cells were z, z+1; the next cell
 *            is forbidden from being z again
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* cell state code c in 0..6: value v = (c+1)/2, flag f = (c>0) ? (c+1)%2 : 0 */
static int enc(int v, int f) { return v == 0 ? 0 : 2 * v - 1 + f; }

seq_t a(idx_t n)
{
    int N = (int)n;
    long long pw[16];
    pw[0] = 1;
    for (int k = 1; k <= N + 1; k++) pw[k] = pw[k - 1] * 7;
    long long size = pw[N + 1];
    seq_t *cur = mem_new(seq_t, size);
    seq_t *nxt = mem_new(seq_t, size);
    cur[0] = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (long long s = 0; s < size; s++) nxt[s] = 0;
            long long pc = pw[1 + j];
            for (long long s = 0; s < size; s++) {
                if (cur[s] == 0) continue;
                int cs = (int)((s / pc) % 7);
                int cv = (cs + 1) / 2, cf = cs > 0 ? (cs + 1) % 2 : 0;
                int rs = (int)(s % 7);
                int rv = (rs + 1) / 2, rf = rs > 0 ? (rs + 1) % 2 : 0;
                for (int x = 0; x < 4; x++) {
                    if (i > 0 && cf && x == cv - 1) continue;
                    if (j > 0 && rf && x == rv - 1) continue;
                    int ncf = (i > 0 && x == cv + 1) ? 1 : 0;
                    int nrf = (j > 0 && x == rv + 1) ? 1 : 0;
                    int nc = enc(x, ncf), nr = enc(x, nrf);
                    long long t = s + (long long)(nc - cs) * pc + (long long)(nr - rs);
                    nxt[t] += cur[s];
                }
            }
            seq_t *tmp = cur; cur = nxt; nxt = tmp;
        }
    }
    seq_t total = 0;
    for (long long s = 0; s < size; s++) total += cur[s];
    mem_free(cur);
    mem_free(nxt);
    return total;
}
