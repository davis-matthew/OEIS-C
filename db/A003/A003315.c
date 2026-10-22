/*
 * A003315  Representing n as sum of increasing powers.
 * https://oeis.org/A003315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n = x_1^2 + x_2^3 + ... + x_k^(k+1)
 *            with all x_i >= 1; computed by a search: build the set of
 *            reachable sums layer by layer (a bitset over 0..n)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   a3315_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a3315_pw.h"

seq_t a(idx_t n) {
    unsigned long long N = (unsigned long long)n;
    unsigned char *cur, *nxt, *tmp;
    unsigned long long x, s, k, e;
    seq_t res;
    if (N == 0) return 0;
    cur = (unsigned char *)calloc((size_t)N + 1, 1);
    nxt = (unsigned char *)calloc((size_t)N + 1, 1);
    for (x = 1; x * x <= N; x++) cur[x * x] = 1;
    k = 1;
    while (!cur[N]) {
        e = k + 2;
        memset(nxt, 0, (size_t)N + 1);
        for (x = 1;; x++) {
            unsigned long long p = (x == 1) ? 1 : a3315_pw(x, e, N);
            if (p > N) break;
            for (s = k; s + p <= N; s++)
                if (cur[s]) nxt[s + p] = 1;
        }
        tmp = cur; cur = nxt; nxt = tmp;
        k++;
    }
    free(cur);
    free(nxt);
    res = (seq_t)k;
    return res;
}
