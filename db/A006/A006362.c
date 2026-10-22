/*
 * A006362  Antichains (or order ideals) in the poset 2*2*5*n or size of the
 *          distributive lattice J(2*2*5*n).
 * https://oeis.org/A006362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..min(n,21)} s(k)*binomial(n-1,k-1),
 *            where s(k) is the number of strict chains of k order ideals in
 *            J(2*2*5) (computed by DP over its 196 ideals)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t s_chain[22];
static bool s_done = false;

static void s_init(void) {
    static uint32_t ideals[256];
    static seq_t g[256], h[256];
    int cnt = 0;
    uint32_t mask;
    int e, x, y, k;

    for (mask = 0; mask < (1u << 20); mask++) {
        bool ok = true;
        for (e = 0; e < 20 && ok; e++) {
            int i, j, l;
            if (!(mask & (1u << e))) continue;
            i = e / 10; j = (e / 5) % 2; l = e % 5;
            if (i > 0 && !(mask & (1u << (e - 10)))) ok = false;
            if (j > 0 && !(mask & (1u << (e - 5)))) ok = false;
            if (l > 0 && !(mask & (1u << (e - 1)))) ok = false;
        }
        if (ok) ideals[cnt++] = mask;
    }

    for (x = 0; x < cnt; x++) g[x] = 1;
    for (k = 0; k < 22; k++) s_chain[k] = 0;
    for (k = 1; k <= 21; k++) {
        seq_t tot = 0;
        for (x = 0; x < cnt; x++) tot += g[x];
        s_chain[k] = tot;
        if (k == 21) break;
        for (x = 0; x < cnt; x++) {
            seq_t acc = 0;
            for (y = 0; y < cnt; y++) {
                if (y != x && (ideals[y] & ~ideals[x]) == 0) acc += g[y];
            }
            h[x] = acc;
        }
        for (x = 0; x < cnt; x++) g[x] = h[x];
    }
    s_done = true;
}

seq_t a(idx_t n) {
    seq_t res = 0, c = 1;
    idx_t m, j, kmax;
    if (n == 0) return 1;
    if (!s_done) s_init();
    m = n - 1;
    kmax = n < 21 ? n : 21;
    /* c = binomial(m, j) for j = k-1 */
    for (j = 0; j < kmax; j++) {
        res += s_chain[j + 1] * c;
        c = c * ((seq_t)m - (seq_t)j) / ((seq_t)j + 1);
    }
    return res;
}
