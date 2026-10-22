/*
 * A000366  Genocchi numbers of second kind (A005439) divided by 2^(n-1).
 * https://oeis.org/A000366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of Dellac configurations, computed with Knuth's
 *            continued-fraction g.f. 1/(1 - x/(1 - x/(1 - 3x/(1 - 3x/(1 -
 *            6x/(1 - 6x/...)))))) with weights alpha(h) = k(k+1)/2, k =
 *            ceil(h/2), i.e. a(n+1) = [x^n] of the J/S-fraction, evaluated as
 *            a weighted Motzkin/Dyck path count; a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 26,
 *            big any n
 * Helpers:   alpha
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alpha.h"

#define A366_CAP 10050

static seq_t memo[A366_CAP + 2];
static seq_t bufA[A366_CAP + 3];
static seq_t bufB[A366_CAP + 3];
static idx_t have = 0;

static void compute_upto(idx_t M) {
    idx_t T = 2 * (M - 1);
    seq_t *cur = bufA;
    seq_t *nxt = bufB;
    idx_t i, s, h;
    for (i = 0; i <= M; i++) memo[i] = 0;
    for (i = 0; i < M + 2; i++) { cur[i] = 0; nxt[i] = 0; }

    cur[0] = 1;
    memo[1] = 1;
    for (s = 0; s < T; s++) {
        idx_t prevlim = (s < T - s) ? s : (T - s);
        idx_t t = s + 1;
        idx_t lim = (t < T - t) ? t : (T - t);
        for (h = (t & 1); h <= lim; h += 2) {
            seq_t v = 0;
            if (h >= 1) v = cur[h - 1];
            if (h + 1 <= prevlim) v += alpha(h + 1) * cur[h + 1];
            nxt[h] = v;
        }
        {
            seq_t *tmp = cur; cur = nxt; nxt = tmp;
        }
        if ((t & 1) == 0) memo[t / 2 + 1] = cur[0];
    }
    have = M;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > A366_CAP) n = A366_CAP;
    if (n <= have) return memo[n];
    compute_upto(n);
    return memo[n];
}
