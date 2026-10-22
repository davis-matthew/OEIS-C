/*
 * A005907  a(n) = floor(tau*a(n-2)) + a(n-1) with a(0)=1 and a(1)=3.
 * https://oeis.org/A005907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(tau*a(n-2)) + a(n-1), a(0) = 1, a(1) = 3, tau =
 *            (1+sqrt(5))/2. Exact integer method: keep b(m) =
 *            floor(tau*a(m)). Then a(n) = a(n-1) + b(n-2) and b(n) = b(n-1) +
 *            b(n-2) + a(n-2) - [tau*a(n) < b(n-1) + b(n-2) + a(n-2)]. The
 *            comparison tau*P > Q is exact: either P^2 + P*Q > Q^2, or a E...
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   grow_zero, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_big.h"

/* returns 1 iff tau*p > q (p, q >= 0; equality only when p = q = 0) */
static int phi_gt(seq_t p, seq_t q) {
    int flip = 0;
    if (seq_is_big()) {
        /* here q ~ tau*p, so 2q - p >= 0: tau*p > q  <=>  p^2 + p*q > q^2 */
        return p * (p + q) > q * q;
    }
    for (;;) {
        seq_t r;
        if (p == 0) return flip;
        if (q < p) return !flip;
        /* tau*p > q  <=>  p > tau*(q-p)  <=>  !(tau*(q-p) > p) */
        r = q - p;
        q = p;
        p = r;
        flip ^= 1;
    }
}

seq_t a(idx_t n) {
    static seq_t *A = NULL;
    static seq_t *B = NULL;
    static idx_t len = 0;
    static idx_t cap = 0;

    if (cap == 0) {
        A = grow_zero(NULL, 0, 16);
        B = grow_zero(NULL, 0, 16);
        cap = 16;
        A[0] = (seq_t)1; A[1] = (seq_t)3;
        B[0] = (seq_t)1; B[1] = (seq_t)4;
        len = 2;
    }
    if (n >= len) {
        if (n >= cap) {
            idx_t nc = cap * 2;
            if (nc < n + 1) nc = n + 1;
            A = grow_zero(A, cap, nc);
            B = grow_zero(B, cap, nc);
            cap = nc;
        }
        for (idx_t i = len; i <= n; i++) {
            seq_t q;
            A[i] = A[i - 1] + B[i - 2];
            q = B[i - 1] + B[i - 2] + A[i - 2];
            if (!phi_gt(A[i], q)) q -= 1;
            B[i] = q;
        }
        len = n + 1;
    }
    return A[n];
}
