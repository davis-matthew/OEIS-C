/*
 * A000753  Boustrophedon transform of Catalan numbers.
 * https://oeis.org/A000753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon transform of Catalan numbers s(n) = A000108(n):
 *            T(n,0) = s(n), T(n,k) = T(n,k-1) + T(n-1,n-k) for 1 <= k <= n,
 *            a(n) = T(n,n). The Catalan numbers come from the ballot triangle
 *            B(r,0) = 1, B(r,k) = B(r,k-1) + B(r-1,k), B(r-1,r) = 0, with
 *            s(r) = B(r,r)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_grow.h"

static seq_t *A_memo = 0;   /* computed terms a(0..cnt-1) */
static seq_t *T_row = 0;    /* Boustrophedon row cnt-1 */
static seq_t *U_row = 0;    /* scratch row */
static seq_t *B_row = 0;    /* ballot row cnt-1 */
static idx_t cnt = 0, cap = 0;

static void ensure_cap(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap * 2;
    if (nc < need) nc = need;
    if (nc < 16) nc = 16;
    A_memo = a306_grow(A_memo, cap, nc);
    T_row = a306_grow(T_row, cap, nc);
    U_row = a306_grow(U_row, cap, nc);
    B_row = a306_grow(B_row, cap, nc);
    cap = nc;
}

seq_t a(idx_t n) {
    idx_t r, k;
    if (n < cnt) return A_memo[n];
    ensure_cap(n + 2);
    for (r = cnt; r <= n; r++) {
        seq_t *tmp;
        /* ballot row r: B_row[r] = Catalan(r) */
        B_row[r] = 0;
        if (r == 0) {
            B_row[0] = 1;
        } else {
            for (k = 1; k <= r; k++) B_row[k] += B_row[k - 1];
        }
        /* Boustrophedon row r */
        U_row[0] = B_row[r];
        for (k = 1; k <= r; k++) U_row[k] = U_row[k - 1] + T_row[r - k];
        tmp = T_row; T_row = U_row; U_row = tmp;
        A_memo[r] = T_row[r];
    }
    cnt = n + 1;
    return A_memo[n];
}
