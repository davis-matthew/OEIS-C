/*
 * A002017  Expansion of e.g.f. exp(sin(x)).
 * https://oeis.org/A002017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} (-1)^k*C(n-1,2k)*a(n-1-2k),
 *            a(0) = 1 (from f' = cos(x)*f for f = exp(sin(x)); same as the
 *            OEIS (n-1)!*Sum ... formula)
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 18, i64 n <= 28, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *A_cache = NULL;
static idx_t A_len = 0;
static idx_t A_cap = 0;

static void ensure_cap(idx_t need) {
    if (need <= A_cap) return;
    idx_t nc = A_cap ? A_cap : 64;
    while (nc < need) nc *= 2;
    seq_t *nb = (seq_t *)malloc(nc * sizeof(seq_t));
    memset((void *)nb, 0, nc * sizeof(seq_t));
    if (A_cache != NULL) {
        memcpy((void *)nb, (void *)A_cache, A_len * sizeof(seq_t));
        free(A_cache);
    }
    A_cache = nb;
    A_cap = nc;
}

seq_t a(idx_t n) {
    if (n < A_len) return A_cache[n];
    ensure_cap(n + 1);
    if (A_len == 0) {
        A_cache[0] = 1;
        A_len = 1;
    }
    for (idx_t m = A_len; m <= n; m++) {
        seq_t s = 0;
        seq_t c = 1; /* C(m-1, j) */
        for (idx_t j = 0; j < m; j++) {
            if ((j & 1) == 0) {
                seq_t term = c * A_cache[m - 1 - j];
                if ((j & 3) == 0) s += term;
                else s -= term;
            }
            if (j + 1 < m) {
                c = c * (seq_t)(m - 1 - j) / (seq_t)(j + 1);
            }
        }
        A_cache[m] = s;
        A_len = m + 1;
    }
    return A_cache[n];
}
