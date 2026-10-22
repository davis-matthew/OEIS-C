/*
 * A001452  Number of 5-line partitions of n.
 * https://oeis.org/A001452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. 1/Product_{k>=1} (1-x^k)^min(k,5) = (1-x)^4 (1-x^2)^3
 *            (1-x^3)^2 (1-x^4) / Product_{k>=1} (1-x^k)^5. Divide by
 *            Product(1-x^k) five times with the pentagonal-number recurrence
 *            b(m) = f(m) + Sum_{i>=1} (-1)^(i+1) (b(m-i(3i-1)/2) +
 *            b(m-i(3i+1)/2)), then multiply by (1-x^4), (1-x^3)^2,
 *            (1-x^2)^3,...
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 52, u64 n <= 144, u128 n <= 435,
 *            big any n
 * Helpers:   a001452_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001452_zeros.h"

static seq_t *a001452_cache = NULL;
static idx_t a001452_len = 0;

static void a001452_build(idx_t len) {
    seq_t *A = a001452_zeros(len);
    seq_t *B = a001452_zeros(len);
    seq_t *t;
    idx_t m, i, j;
    int pass, rep, k;

    A[0] = 1;
    for (pass = 0; pass < 5; pass++) {
        for (m = 0; m < len; m++) {
            seq_t pos = A[m];
            seq_t neg = 0;
            for (i = 1;; i++) {
                idx_t p1 = i * (3 * i - 1) / 2;
                idx_t p2;
                if (p1 > m) break;
                p2 = p1 + i;
                if (i & 1) {
                    pos += B[m - p1];
                    if (p2 <= m) pos += B[m - p2];
                } else {
                    neg += B[m - p1];
                    if (p2 <= m) neg += B[m - p2];
                }
            }
            B[m] = pos - neg;
        }
        t = A; A = B; B = t;
    }
    /* multiply by (1-x^k)^(5-k) for k = 4, 3, 2, 1 */
    for (k = 4; k >= 1; k--) {
        for (rep = 0; rep < 5 - k; rep++) {
            for (j = len - 1; j >= (idx_t)k; j--) {
                A[j] -= A[j - k];
            }
        }
    }
    free(B);
    if (a001452_cache) free(a001452_cache);
    a001452_cache = A;
    a001452_len = len;
}

seq_t a(idx_t n) {
    if (n >= a001452_len) {
        idx_t len = n + 1;
        if (len < 2 * a001452_len) len = 2 * a001452_len;
        if (len < 64) len = 64;
        a001452_build(len);
    }
    return a001452_cache[n];
}
