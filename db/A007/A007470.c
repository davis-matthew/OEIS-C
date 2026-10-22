/*
 * A007470  Shifts left when Stirling2 transform is applied twice.
 * https://oeis.org/A007470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(1) = 1; b(m) = Sum_{k=1..m} Stirling2(m,k)*a(k); a(m+1) =
 *            Sum_{k=1..m} Stirling2(m,k)*b(k) (Stirling2 transform applied
 *            twice shifts left)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *A_ = 0;   /* a[1..cnt] */
static seq_t *B_ = 0;   /* b[1..cnt-1] */
static seq_t *S_ = 0;   /* current Stirling2 row S2(r, 0..r) */
static idx_t cap = 0;
static idx_t cnt = 0;

static void ensure_cap(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    A_ = grow_zero(A_, cap + 1, nc + 1);
    B_ = grow_zero(B_, cap + 1, nc + 1);
    S_ = grow_zero(S_, cap + 1, nc + 1);
    cap = nc;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (cnt == 0) {
        ensure_cap(2);
        A_[1] = 1;
        S_[0] = 1;          /* row 0 of Stirling2 */
        cnt = 1;
    }
    if (n > cnt) ensure_cap(n + 1);
    while (cnt < n) {
        idx_t r = cnt, k;
        seq_t bs, as;
        /* advance Stirling2 row from r-1 to r */
        for (k = r; k >= 1; k--)
            S_[k] = (seq_t)k * S_[k] + S_[k - 1];
        S_[0] = 0;
        bs = 0;
        for (k = 1; k <= r; k++)
            bs += S_[k] * A_[k];
        B_[r] = bs;
        as = 0;
        for (k = 1; k <= r; k++)
            as += S_[k] * B_[k];
        A_[r + 1] = as;
        cnt = r + 1;
    }
    return A_[n];
}
