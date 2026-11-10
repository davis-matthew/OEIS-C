/*
 * A074289  Values of A000002(n)*A054353(n) that are repeated.
 * https://oeis.org/A074289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sorted values v that equal A000002(m)*A054353(m) for at least
 *            two indices m (computed by generating the Kolakoski sequence and
 *            its partial sums, then counting products)
 * Range:     u8 n <= 17, u16 n <= 3721, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9063 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *rep_list = NULL;
static uint64_t rep_cnt = 0;

static void build_rep(uint64_t L)
{
    unsigned char *K = (unsigned char *)malloc((size_t)(L + 8));
    unsigned char *cnt = (unsigned char *)calloc((size_t)(3 * L + 8), 1);
    uint64_t len, run, m, s, c;
    if (!K || !cnt) { free(K); free(cnt); return; }
    K[1] = 1; K[2] = 2; K[3] = 2;
    len = 3; run = 3;
    while (len < L) {
        unsigned char v = (unsigned char)((run & 1) ? 1 : 2);
        unsigned char r = K[run], k;
        for (k = 0; k < r && len < L + 4; k++) K[++len] = v;
        run++;
    }
    s = 0;
    for (m = 1; m <= L; m++) {
        s += K[m];
        c = (uint64_t)K[m] * s;
        if (cnt[c] < 2) cnt[c]++;
    }
    c = 0;
    for (m = 1; m <= L; m++) if (cnt[m] >= 2) c++;
    free(rep_list);
    rep_list = (uint64_t *)malloc((size_t)(c + 1) * sizeof(uint64_t));
    if (rep_list) {
        rep_cnt = 0;
        for (m = 1; m <= L; m++) if (cnt[m] >= 2) rep_list[rep_cnt++] = m;
    } else rep_cnt = 0;
    free(K);
    free(cnt);
}

seq_t a(idx_t n)
{
    static uint64_t lim = 0;
    if (n < 1) return (seq_t)0;
    while (rep_cnt < (uint64_t)n) {
        lim = lim ? lim * 2 : 1024;
        build_rep(lim);
        if (!rep_list) return (seq_t)0;
    }
    return (seq_t)rep_list[n - 1];
}
