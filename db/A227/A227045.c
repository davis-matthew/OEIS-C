/*
 * A227045  G.f.: 1/(1 - q/G(0)) where G(k) = 1 - q^(k+1) / (1 - q^(k+1) /
 *          G(k+1) ).
 * https://oeis.org/A227045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. A(q) = 1/(1 - q/(1 - q/(1 - q/(1 - q^2/(1 - q^2/(1 - q^3/(1
 *            - q^3/(1 - ...)))))))), * i.e. the continued fraction with
 *            partial numerators q^e_k for e = 1, 1,1, 2,2, 3,3, ... *
 *            Evaluated bottom-up as H_k = 1/(1 - q^e_k * H_{k+1}) = 1 + q^e_k
 *            * H_{k+1} * H_k, * so every coefficient is a sum of...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 836 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A227045_cache = NULL;
static idx_t A227045_len = 0;

static void A227045_build(idx_t M) {
    idx_t L = M + 1;
    idx_t cap = 16, cnt = 0, S = 0, k, i, j;
    idx_t *ex = (idx_t *)malloc((size_t)cap * sizeof(idx_t));
    idx_t *pre = (idx_t *)malloc((size_t)cap * sizeof(idx_t));
    seq_t *H, *Hn, *tmp;

    /* exponent list 1, 1,1, 2,2, 3,3, ... ; keep level k while S_k <= M */
    while (S <= M) {
        idx_t e;
        if (cnt == 0) e = 1;
        else e = (cnt - 1) / 2 + 1;
        if (cnt == cap) {
            cap *= 2;
            ex = (idx_t *)realloc(ex, (size_t)cap * sizeof(idx_t));
            pre = (idx_t *)realloc(pre, (size_t)cap * sizeof(idx_t));
        }
        ex[cnt] = e;
        pre[cnt] = S;
        cnt++;
        S += e;
    }

    H = mem_new(seq_t, (size_t)L);
    Hn = mem_new(seq_t, (size_t)L);
    for (i = 0; i < L; i++) { H[i] = 0; Hn[i] = 0; }
    H[0] = 1; /* truncation of the tail */

    for (k = cnt; k-- > 0;) {
        idx_t e = ex[k];
        idx_t P = M - pre[k];
        Hn[0] = 1;
        for (i = 1; i <= P; i++) {
            seq_t s = 0;
            if (i >= e) {
                idx_t t = i - e;
                for (j = 0; j <= t; j++) s += H[j] * Hn[t - j];
            }
            Hn[i] = s;
        }
        tmp = H; H = Hn; Hn = tmp;
    }

    free(ex);
    free(pre);
    for (i = 0; i < L; i++) Hn[i] = 0;
    mem_free(Hn);

    if (A227045_cache != NULL) {
        for (i = 0; i < A227045_len; i++) A227045_cache[i] = 0;
        mem_free(A227045_cache);
    }
    A227045_cache = H;
    A227045_len = L;
}

seq_t a(idx_t n) {
    if (n >= A227045_len) {
        idx_t M = n;
        if (A227045_len > 0 && 2 * A227045_len > M) M = 2 * A227045_len;
        if (M < 32) M = 32;
        A227045_build(M);
    }
    return A227045_cache[n];
}
