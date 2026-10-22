/*
 * A005640  Number of phylogenetic trees with n labels.
 * https://oeis.org/A005640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   For n >= 3, a(n) = 2^n*A000311(n-1), with a(0) = 1, a(1) = 1,
 *            a(2) = 2, where A000311(1) = A000311(2) = 1 and, for m >= 2,
 *            A000311(m+1) = (m+2)*A000311(m) + 2*Sum_{k=2..m-1}
 *            binomial(m,k)*A000311(k)*A000311(m+1-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   grow_zero, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zalloc_arr.h"
#include "../utils/grow_zero.h"

static seq_t *sb = NULL;   /* sb[i] = A000311(i) for i < sb_len */
static idx_t sb_len = 0;
static seq_t *srow = NULL; /* srow[k] = binomial(srow_n, k) */
static idx_t srow_cap = 0;
static idx_t srow_n = 0;

static void ensure_b(idx_t m) {
    if (sb == NULL) {
        idx_t cap = m + 1 < 3 ? 3 : m + 1;
        sb = zalloc_arr(cap);
        sb[0] = 0;
        sb[1] = 1;
        sb[2] = 1;
        sb_len = 3;
        srow_cap = cap + 1;
        srow = zalloc_arr(srow_cap);
        srow[0] = 1;
        srow[1] = 2;
        srow[2] = 1;
        srow_n = 2;
    }
    if (m < sb_len) return;
    sb = grow_zero(sb, sb_len, m + 1);
    if (srow_cap < m + 2) {
        srow = grow_zero(srow, srow_cap, m + 2);
        srow_cap = m + 2;
    }
    for (idx_t t = sb_len; t <= m; t++) {
        idx_t nn = t - 1; /* nn >= 2 */
        while (srow_n < nn) {
            idx_t r = srow_n;
            srow[r + 1] = 0;
            for (idx_t k = r + 1; k >= 1; k--) srow[k] += srow[k - 1];
            srow_n = r + 1;
        }
        seq_t s = 0;
        for (idx_t k = 2; k + 1 <= nn; k++) {
            s += srow[k] * sb[k] * sb[nn + 1 - k];
        }
        sb[t] = ((seq_t)nn + 2) * sb[nn] + s + s;
    }
    sb_len = m + 1;
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    if (n == 2) return 2;
    ensure_b(n - 1);
    seq_t r = sb[n - 1];
    for (idx_t i = 0; i < n; i++) r += r;
    return r;
}
