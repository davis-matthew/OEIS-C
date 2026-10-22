/*
 * A007286  E.g.f.: (sin x + cos 2x) / cos 3x.
 * https://oeis.org/A007286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. A(x) satisfies A(x)*cos(3x) = sin(x) + cos(2x), so a(n) =
 *            [n odd]*(-1)^((n-1)/2) + [n even]*(-1)^(n/2)*2^n -
 *            Sum_{k=1..floor(n/2)} (-1)^k * 9^k * binomial(n,2k) * a(n-2k),
 *            a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   grow_zero, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"
#include "../utils/pw2.h"
#include <stdlib.h>

static seq_t *A_memo = NULL;
static seq_t *row = NULL;   /* row[i] = binomial(have-1, i) */
static idx_t have = 0;      /* number of terms computed */
static idx_t cap = 0;

seq_t a(idx_t n) {
    if (n + 1 > cap) {
        idx_t nc = (n + 1 > 2 * cap) ? n + 1 : 2 * cap;
        if (nc < 16) nc = 16;
        A_memo = grow_zero(A_memo, cap, nc);
        row = grow_zero(row, cap, nc);
        cap = nc;
    }
    if (have == 0) {
        A_memo[0] = 1;
        row[0] = 1;
        have = 1;
    }
    for (idx_t m = have; m <= n; m++) {
        /* advance Pascal row to row m */
        row[m] = 0;
        for (idx_t i = m; i >= 1; i--) row[i] = row[i] + row[i - 1];

        seq_t pos = 0, neg = 0;
        if (m % 2 == 1) {
            if (((m - 1) / 2) % 2 == 0) pos += 1; else neg += 1;
        } else {
            seq_t p2 = pw2(m);
            if ((m / 2) % 2 == 0) pos += p2; else neg += p2;
        }
        seq_t p9 = 1;
        for (idx_t k = 1; 2 * k <= m; k++) {
            p9 *= 9;
            idx_t j = m - 2 * k;
            seq_t t = p9 * row[j] * A_memo[j];
            if (k % 2 == 1) pos += t; else neg += t;
        }
        A_memo[m] = pos - neg;
        have = m + 1;
    }
    return A_memo[n];
}
