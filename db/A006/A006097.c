/*
 * A006097  Gaussian binomial coefficient [n, 4] for q = 2.
 * https://oeis.org/A006097
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   q-Pascal recurrence for q = 2: G(n,k) = G(n-1,k-1) +
 *            2^k*G(n-1,k), G(0,0) = 1, G(0,k) = 0 for k > 0, G(n,0) = 1; a(n)
 *            = G(n,4) (equivalently a(n) =
 *            (2^n-1)*(2^n-2)*(2^n-4)*(2^n-8)/20160)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 196 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *A_memo = NULL;
static idx_t A_cap = 0;
static idx_t A_cnt = 0;
static seq_t G_row[5];

seq_t a(idx_t n) {
    idx_t k;
    if (n < A_cnt) return A_memo[n];
    if (n + 1 > A_cap) {
        idx_t newcap = A_cap * 2;
        if (newcap < n + 1) newcap = n + 1;
        A_memo = grow_zero(A_memo, A_cap, newcap);
        A_cap = newcap;
    }
    if (A_cnt == 0) {
        G_row[0] = 1;
        for (k = 1; k <= 4; k++) G_row[k] = 0;
    }
    while (A_cnt <= n) {
        if (A_cnt > 0) {
            for (k = 4; k >= 1; k--) {
                seq_t mult = 1;
                mult <<= k;
                G_row[k] = G_row[k - 1] + G_row[k] * mult;
            }
        }
        A_memo[A_cnt] = G_row[4];
        A_cnt++;
    }
    return A_memo[n];
}
