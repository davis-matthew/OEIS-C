/*
 * A001294  Triangle in which k-th number (0<=k<=n) in n-th row (0<=n) is
 *          number of dodecads in Golay code G_24 containing k given points
 *          and missing n-k given points.
 * https://oeis.org/A001294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, fini, full, nice
 * Formula:   search over all 4096 codewords of the extended Golay code G_24
 *            (built from generator polynomial x^11+x^10+x^6+x^5+x^4+x^2+1
 *            plus a parity bit). Fix the points of one octad, p_1..p_8.
 *            T(n,k) = number of dodecads (weight-12 codewords) that contain
 *            p_1..p_k and miss p_{k+1}..p_n
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   g24_pop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/g24_pop.h"

static int g24_ready = 0;
static long long g24_tab[9][9];

static void g24_init(void) {
    uint32_t cw[4096];
    uint32_t g = 0xC75; /* bits 0,2,4,5,6,10,11 */
    uint32_t octad = 0, pts[8], m;
    int i, j, k, n, np = 0;
    for (m = 0; m < 4096; m++) {
        uint32_t c = 0;
        for (i = 0; i < 12; i++)
            if (m & (1u << i)) c ^= g << i;
        if (g24_pop(c) & 1) c |= 1u << 23;
        cw[m] = c;
        if (!octad && g24_pop(c) == 8) octad = c;
    }
    for (i = 0; i < 24; i++)
        if (octad & (1u << i)) pts[np++] = (uint32_t)1 << i;
    for (n = 0; n <= 8; n++) {
        for (k = 0; k <= n; k++) {
            uint32_t in = 0, out = 0;
            long long cnt = 0;
            for (j = 0; j < k; j++) in |= pts[j];
            for (j = k; j < n; j++) out |= pts[j];
            for (m = 0; m < 4096; m++) {
                uint32_t c = cw[m];
                if (g24_pop(c) != 12) continue;
                if ((c & in) == in && (c & out) == 0) cnt++;
            }
            g24_tab[n][k] = cnt;
        }
    }
    g24_ready = 1;
}

seq_t a(idx_t n) {
    idx_t row = 0, start = 0;
    if (!g24_ready) g24_init();
    while (start + row + 1 <= n) {
        start += row + 1;
        row++;
        if (row > 8) return (seq_t)0;
    }
    {
        idx_t col = n - start;
        seq_t r = (seq_t)0;
        r += (seq_t)g24_tab[row][col];
        return r;
    }
}
