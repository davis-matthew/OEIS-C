/*
 * A004121  Generalized weak orders on n points.
 * https://oeis.org/A004121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   E.g.f.: 1/(1 - Sum_{i >= 1} 2^binomial(i+1, 2)*x^i/i!), i.e.
 *            a(n) = 2^binomial(n+1,2) + Sum_{j=1..n-1}
 *            binomial(n,j)*a(j)*2^binomial(n-j+1,2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t shl_seq(seq_t x, idx_t e) {
    if (seq_is_fixed_width()) {
        idx_t w = (idx_t)(sizeof(seq_t) * 8);
        if (e >= w) return (seq_t)0;
    }
    return x << e;
}

seq_t a(idx_t n) {
    static seq_t *A = NULL;   /* A[k] = a(k) */
    static seq_t *C = NULL;   /* C[j] = binomial(done, j) */
    static idx_t done = 0, cap = 0;

    if (n <= done && A != NULL) return A[n];

    if (n + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        A = grow_zero(A, cap, nc);
        C = grow_zero(C, cap, nc);
        if (cap == 0) C[0] = (seq_t)1;
        cap = nc;
    }

    for (idx_t k = done + 1; k <= n; k++) {
        /* advance Pascal row from k-1 to k */
        for (idx_t j = k; j >= 1; j--) {
            C[j] += C[j - 1];
        }
        seq_t v = shl_seq((seq_t)1, k * (k + 1) / 2);
        for (idx_t j = 1; j < k; j++) {
            idx_t m = k - j;
            v += shl_seq(C[j] * A[j], m * (m + 1) / 2);
        }
        A[k] = v;
        done = k;
    }
    return A[n];
}
