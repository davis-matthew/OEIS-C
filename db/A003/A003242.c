/*
 * A003242  Number of compositions of n such that no two adjacent parts are
 *          equal (these are sometimes called Carlitz compositions).
 * https://oeis.org/A003242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=1..n} A048272(k)*a(n-k), a(0) = 1, where
 *            A048272(k) = (number of odd divisors of k) - (number of even
 *            divisors of k)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 80, u128 n <= 159,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"

static seq_t *A_vals = NULL;
static int *C_vals = NULL;
static idx_t A_cap = 0;
static idx_t A_have = 0;

seq_t a(idx_t n) {
    if (n >= A_cap) {
        idx_t nc = A_cap ? A_cap : 16;
        idx_t d, m;
        while (nc <= n) nc *= 2;
        A_vals = A_grow(A_vals, A_cap, nc);
        free(C_vals);
        C_vals = (int *)calloc((size_t)nc, sizeof(int));
        for (d = 1; d < nc; d++) {
            int s = (d & 1) ? 1 : -1;
            for (m = d; m < nc; m += d) C_vals[m] += s;
        }
        A_cap = nc;
    }
    if (A_have == 0) {
        A_vals[0] = (seq_t)1;
        A_have = 1;
    }
    while (A_have <= n) {
        idx_t i = A_have, k;
        seq_t pos = (seq_t)0, neg = (seq_t)0;
        for (k = 1; k <= i; k++) {
            int c = C_vals[k];
            if (c > 0) {
                if (c == 1) pos += A_vals[i - k];
                else pos += A_vals[i - k] * (idx_t)c;
            } else if (c < 0) {
                if (c == -1) neg += A_vals[i - k];
                else neg += A_vals[i - k] * (idx_t)(-c);
            }
        }
        A_vals[i] = pos - neg;
        A_have++;
    }
    return A_vals[n];
}
