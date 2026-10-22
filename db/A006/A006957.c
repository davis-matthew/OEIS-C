/*
 * A006957  Self-convolution of numbers of preferential arrangements.
 * https://oeis.org/A006957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{i=0..n} F(i)*F(n-i), where F = A000670 (Fubini
 *            numbers), F(m) = Sum_{k=0..m} T(m,k), T(m,k) =
 *            k!*Stirling2(m,k), T(m,k) = k*(T(m-1,k) + T(m-1,k-1)), T(0,0) =
 *            1, T(m,0) = 0 for m >= 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *F_memo = 0;   /* Fubini numbers F(0..cnt-1) */
static seq_t *T_row = 0;    /* T(cnt-1, 0..cnt-1) */
static idx_t cnt = 0;
static idx_t cap = 0;

static void ensure(idx_t n) {
    if (cnt > n) return;
    if (cap < n + 1) {
        idx_t nc = cap ? cap : 16;
        while (nc < n + 1) nc *= 2;
        if (cap == 0) {
            F_memo = alloc_zero(nc);
            T_row = alloc_zero(nc);
        } else {
            F_memo = grow_zero(F_memo, cap, nc);
            T_row = grow_zero(T_row, cap, nc);
        }
        cap = nc;
    }
    if (cnt == 0) {
        T_row[0] = 1;
        F_memo[0] = 1;
        cnt = 1;
    }
    while (cnt <= n) {
        idx_t m = cnt;
        seq_t s = 0;
        for (idx_t k = m; k >= 1; k--) {
            T_row[k] = (T_row[k] + T_row[k - 1]) * (seq_t)k;
            s += T_row[k];
        }
        T_row[0] = 0;
        F_memo[m] = s;
        cnt++;
    }
}

seq_t a(idx_t n) {
    ensure(n);
    seq_t result = 0;
    for (idx_t i = 0; i <= n; i++) {
        result += F_memo[i] * F_memo[n - i];
    }
    return result;
}
