/*
 * A002718  Number of bicoverings of an n-set.
 * https://oeis.org/A002718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) is computed with a dynamic program over the points 1..n.
 *            Blocks are put in canonical order by their smallest element. Two
 *            blocks created at the same point stay a "pending pair" until a
 *            later point tells them apart. The state is (d, q) = (number of
 *            blocks already told apart, number of pending...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    size_t D = (size_t)(2 * n + 1);
    size_t Q = (size_t)(n + 2);
    size_t total = D * Q;
    seq_t *cur = (seq_t *)calloc(total, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(total, sizeof(seq_t));
    size_t i;
    for (i = 0; i < total; i++) { cur[i] = 0; nxt[i] = 0; }
    cur[0] = 1;

    idx_t p, q, d;
    for (p = 0; p < n; p++) {
        idx_t rem = n - p;
        idx_t qmax_new = p + 1;
        idx_t dmax_new = 2 * p + 2;
        if (dmax_new > 2 * n) dmax_new = 2 * n;
        for (q = 0; q <= qmax_new; q++)
            for (d = 0; d <= dmax_new; d++)
                nxt[(size_t)q * D + (size_t)d] = 0;

        for (q = 0; q <= p; q++) {
            if (q > 2 * rem) break;
            for (d = 0; d <= 2 * p && d < D; d++) {
                seq_t v = cur[(size_t)q * D + (size_t)d];
                if (v == 0) continue;
                size_t base = (size_t)q * D + (size_t)d;
                /* two distinct old told-apart blocks, or both blocks of one pending pair */
                idx_t c = d * (d - (d > 0 ? 1 : 0)) / 2 + q;
                if (d < 2) c = q;
                if (c) nxt[base] += v * (seq_t)c;
                /* one old told-apart block and the first block of a pending pair */
                if (d > 0 && q > 0)
                    nxt[(size_t)(q - 1) * D + (size_t)(d + 2)] += v * (seq_t)(d * q);
                /* first blocks of two different pending pairs */
                if (q >= 2)
                    nxt[(size_t)(q - 2) * D + (size_t)(d + 4)] += v * (seq_t)(q * (q - 1) / 2);
                /* one old told-apart block and one new block */
                if (d > 0)
                    nxt[base + 1] += v * (seq_t)d;
                /* first block of a pending pair and one new block */
                if (q > 0)
                    nxt[(size_t)(q - 1) * D + (size_t)(d + 3)] += v * (seq_t)q;
                /* two new blocks, making a new pending pair */
                nxt[(size_t)(q + 1) * D + (size_t)d] += v;
            }
        }
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    seq_t res = 0;
    for (d = 0; d <= 2 * n; d++) res += cur[(size_t)d];
    free(cur);
    free(nxt);
    return res;
}
