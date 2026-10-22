/*
 * A005556  Exponents m_i associated with Weyl group W(E6).
 * https://oeis.org/A005556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th exponent of W(E6). Computed by generating the 36
 *            positive roots of E6 from its Cartan matrix and taking c_k =
 *            #{positive roots of height k}. Exponent k occurs c_k - c_{k+1}
 *            times, which gives the dual partition
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

static int e6_cartan[6][6];
static int e6_roots[64][6];
static int e6_nroots = 0;
static int e6_exps[16];
static int e6_nexps = 0;
static int e6_done = 0;

static int e6_find(const int *v)
{
    int i;
    for (i = 0; i < e6_nroots; i++)
        if (memcmp(e6_roots[i], v, sizeof(int) * 6) == 0) return 1;
    return 0;
}

static void e6_init(void)
{
    int edges[5][2] = { {0, 2}, {2, 3}, {3, 4}, {4, 5}, {1, 3} };
    int i, j, k, idx;
    int cnt[16];

    memset(e6_cartan, 0, sizeof(e6_cartan));
    for (i = 0; i < 6; i++) e6_cartan[i][i] = 2;
    for (i = 0; i < 5; i++) {
        e6_cartan[edges[i][0]][edges[i][1]] = -1;
        e6_cartan[edges[i][1]][edges[i][0]] = -1;
    }

    e6_nroots = 0;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) e6_roots[e6_nroots][j] = (i == j);
        e6_nroots++;
    }

    for (idx = 0; idx < e6_nroots; idx++) {
        for (i = 0; i < 6; i++) {
            int v[6], p = 0, pairing = 0, q;
            /* p = largest p with beta - p*alpha_i a root */
            for (;;) {
                memcpy(v, e6_roots[idx], sizeof(v));
                v[i] -= p + 1;
                if (v[i] < 0 || !e6_find(v)) break;
                p++;
            }
            for (j = 0; j < 6; j++) pairing += e6_roots[idx][j] * e6_cartan[j][i];
            q = p - pairing;
            if (q > 0) {
                memcpy(v, e6_roots[idx], sizeof(v));
                v[i] += 1;
                if (!e6_find(v) && e6_nroots < 64) {
                    memcpy(e6_roots[e6_nroots], v, sizeof(v));
                    e6_nroots++;
                }
            }
        }
    }

    for (k = 0; k < 16; k++) cnt[k] = 0;
    for (idx = 0; idx < e6_nroots; idx++) {
        int h = 0;
        for (j = 0; j < 6; j++) h += e6_roots[idx][j];
        if (h < 15) cnt[h]++;
    }

    e6_nexps = 0;
    for (k = 1; k < 15; k++) {
        int m = cnt[k] - cnt[k + 1];
        while (m-- > 0 && e6_nexps < 16) e6_exps[e6_nexps++] = k;
    }
    e6_done = 1;
}

seq_t a(idx_t n)
{
    if (!e6_done) e6_init();
    if (n < 1 || n > (idx_t)e6_nexps) return (seq_t)0;
    return (seq_t)e6_exps[n - 1];
}
