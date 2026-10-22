/*
 * A007017  Number of motifs in triangular window of side n.
 * https://oeis.org/A007017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form; a(n) is computed by a transfer-matrix
 *            (broken-profile) DP. It counts 0/1 fillings of a triangle with
 *            n+1 rows that have no all-equal upright or inverted 2x2x2
 *            subtriangle. The top cell is fixed to 0, so the count is already
 *            half the total, as OEIS defines it
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n)
{
    size_t m = (size_t)n + 1;          /* rows 0..m-1, row i has i+1 cells */
    size_t maxlen = (size_t)1 << m;
    seq_t *cur = alloc_zero((idx_t)maxlen);
    seq_t *nxt = alloc_zero((idx_t)maxlen);
    seq_t *tmp;
    seq_t total = 0;
    size_t len = 2, st, k, i, j;

    cur[0] = 1;                        /* top cell fixed to 0 (complement symmetry) */

    for (i = 0; i + 1 < m; i++) {
        /* begin row i+1: shift previous row up one bit, place s[0] at bit 0 */
        size_t nlen = len * 2;
        for (k = 0; k < nlen; k++) nxt[k] = 0;
        for (st = 0; st < len; st++) {
            if (cur[st] != 0) {
                nxt[st << 1] += cur[st];
                nxt[(st << 1) | 1] += cur[st];
            }
        }
        tmp = cur; cur = nxt; nxt = tmp;
        len = nlen;

        /* place s[j] for j = 1..i+1; bit j-1 = s[j-1], bit j = r[j-1], bit j+1 = r[j] */
        for (j = 1; j <= i + 1; j++) {
            for (k = 0; k < len; k++) nxt[k] = 0;
            for (st = 0; st < len; st++) {
                unsigned p, q, r2, b;
                if (cur[st] == 0) continue;
                p = (unsigned)((st >> (j - 1)) & 1);
                q = (unsigned)((st >> j) & 1);
                r2 = (j <= i) ? (unsigned)((st >> (j + 1)) & 1) : 2u;
                for (b = 0; b < 2; b++) {
                    size_t ns;
                    if (p == q && q == b) continue;              /* upright triangle */
                    if (j <= i && q == r2 && r2 == b) continue;  /* inverted triangle */
                    ns = (st & ~((size_t)1 << j)) | ((size_t)b << j);
                    nxt[ns] += cur[st];
                }
            }
            tmp = cur; cur = nxt; nxt = tmp;
        }
    }

    for (st = 0; st < len; st++) total += cur[st];
    release(cur, (idx_t)maxlen);
    release(nxt, (idx_t)maxlen);
    return total;
}
