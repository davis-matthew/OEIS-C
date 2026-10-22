/*
 * A001382  Weight distribution of ternary [ 24,12,9 ] quadratic residue code
 *          (also of Pless symmetry code).
 * https://oeis.org/A001382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of codewords of weight 3n in the extended ternary
 *            [24,12,9] quadratic residue code; the generator polynomial g(x)
 *            = gcd(e(x), x^23-1) is found from a QR idempotent e = a +
 *            b*sum_{r QR} x^r + c*sum_{r NR} x^r over GF(3), all 3^12
 *            codewords are enumerated, and the weights are counted
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   a1382_deg, a1382_gcd, a1382_mod, a1382_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/a1382_deg.h"
#include "../utils/a1382_gcd.h"
#include "../utils/a1382_mod.h"
#include "../utils/a1382_rec.h"

static uint64_t a1382_cnt[25];
static int a1382_done = 0;

static void a1382_init(void)
{
    int isqr[23];
    int i, j, a0, b0, c0;
    int g[24];
    int found = 0;
    memset(isqr, 0, sizeof isqr);
    for (i = 1; i < 23; i++)
        isqr[(i * i) % 23] = 1;
    for (a0 = 0; a0 < 3 && !found; a0++)
        for (b0 = 0; b0 < 3 && !found; b0++)
            for (c0 = 0; c0 < 3 && !found; c0++) {
                int e[24], m[24];
                memset(e, 0, sizeof e);
                memset(m, 0, sizeof m);
                e[0] = a0;
                for (i = 1; i < 23; i++)
                    e[i] = isqr[i] ? b0 : c0;
                if (a1382_deg(e) < 0) continue;
                m[23] = 1;
                m[0] = 2; /* x^23 - 1 */
                a1382_gcd(m, e, g);
                if (a1382_deg(g) == 11) found = 1;
            }
    {
        int G[12][24];
        int cur[13][24];
        memset(G, 0, sizeof G);
        for (i = 0; i < 12; i++) {
            int s = 0;
            for (j = 0; j <= 11; j++)
                G[i][i + j] = g[j];
            for (j = 0; j < 23; j++)
                s += G[i][j];
            G[i][23] = (3 - s % 3) % 3;
        }
        memset(cur, 0, sizeof cur);
        memset(a1382_cnt, 0, sizeof a1382_cnt);
        a1382_rec(0, G, cur, a1382_cnt);
    }
    a1382_done = 1;
}

seq_t a(idx_t n)
{
    seq_t r = 0;
    if (n > 8) return r;
    if (!a1382_done) a1382_init();
    r += (idx_t)a1382_cnt[3 * n];
    return r;
}
