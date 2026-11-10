/*
 * A083931  a(n) = A000695(A014486(n)).
 * https://oeis.org/A083931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000695(A014486(n)); the n-th Dyck word is found by
 *            ranking/unranking with ballot-number counts, then its bits are
 *            spread to powers of 4 (Moser-de Bruijn)
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define DW_MAXR 130

static uint64_t dw_ways[DW_MAXR + 1][DW_MAXR + 2];
static bool dw_init = false;

static uint64_t dw_sat_add(uint64_t x, uint64_t y) {
    uint64_t s = x + y;
    return (s < x) ? UINT64_MAX : s;
}

static void dw_build(void) {
    int r, h;
    for (r = 0; r <= DW_MAXR; r++)
        for (h = 0; h < DW_MAXR + 2; h++)
            dw_ways[r][h] = 0;
    dw_ways[0][0] = 1;
    for (r = 1; r <= DW_MAXR; r++) {
        for (h = 0; h <= DW_MAXR; h++) {
            uint64_t v = dw_ways[r - 1][h + 1];
            if (h > 0) v = dw_sat_add(v, dw_ways[r - 1][h - 1]);
            dw_ways[r][h] = v;
        }
    }
    dw_init = true;
}

seq_t a(idx_t n) {
    if (!dw_init) dw_build();
    uint64_t rank = (uint64_t)n;
    int k = 0;
    while (true) {
        uint64_t c = dw_ways[2 * k][0];
        if (rank < c) break;
        rank -= c;
        k++;
    }
    if (k == 0) return (seq_t)0;
    int L = 2 * k;
    seq_t result = (seq_t)1;  /* first bit is 1 */
    int h = 1;
    for (int pos = 1; pos < L; pos++) {
        int r = L - pos - 1;  /* steps remaining after this one */
        int bit;
        uint64_t cnt = 0;
        if (h > 0) cnt = dw_ways[r][h - 1];
        if (h > 0 && rank < cnt) {
            bit = 0;
            h--;
        } else {
            rank -= cnt;
            bit = 1;
            h++;
        }
        result = result * 4 + (seq_t)bit;
    }
    return result;
}
