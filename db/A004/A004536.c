/*
 * A004536  Theta series of 15-dimensional unimodular lattice A15+.
 * https://oeis.org/A004536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   theta series of the odd unimodular lattice A15+ in dimension 15:
 *            Theta = theta_3^15 - 30*theta_3^7*Delta_8, where Delta_8 =
 *            q*prod_{m>=1}(1-q^(2m-1))^8*(1-q^(4m))^8 =
 *            q*psi(q^2)^4*theta_4(q)^4, psi(x) = Sum_{k>=0} x^(k(k+1)/2). So
 *            a(n) = [q^n]theta_3^15 -
 *            30*[q^(n-1)](theta_3^7*theta_4^4*psi(q^2)...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_zero, mul_psi2, mul_theta3, mul_theta4, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/mul_psi2.h"
#include "../utils/mul_theta3.h"
#include "../utils/mul_theta4.h"

static seq_t *S_arr = 0;   /* theta_3^15 */
static seq_t *P_arr = 0;   /* positive part of theta_3^7*theta_4^4*psi(q^2)^4 */
static seq_t *M_arr = 0;   /* negative part */
static idx_t cap = 0;

static void build(idx_t L) {
    idx_t i;
    int j;
    if (S_arr) {
        release(S_arr, cap);
        release(P_arr, cap);
        release(M_arr, cap);
    }
    S_arr = alloc_zero(L);
    P_arr = alloc_zero(L);
    M_arr = alloc_zero(L);
    cap = L;

    S_arr[0] = 1;
    for (j = 0; j < 7; j++) mul_theta3(S_arr, L);
    for (i = 0; i < L; i++) P_arr[i] = S_arr[i];
    for (j = 0; j < 8; j++) mul_theta3(S_arr, L);

    for (j = 0; j < 4; j++) mul_theta4(P_arr, M_arr, L);
    for (j = 0; j < 4; j++) {
        mul_psi2(P_arr, L);
        mul_psi2(M_arr, L);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n + 1 > cap) {
        idx_t L = cap + cap / 4;
        if (L < n + 1) L = n + 1;
        if (L < 64) L = 64;
        build(L);
    }
    seq_t r = S_arr[n];
    r += 30 * M_arr[n - 1];
    r -= 30 * P_arr[n - 1];
    return r;
}
