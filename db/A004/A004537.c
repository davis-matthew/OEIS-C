/*
 * A004537  Theta series of 23-dimensional shorter Leech lattice.
 * https://oeis.org/A004537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   G.f.: theta_3(q)^23 - 46*theta_3(q)^15*Delta_8(q), where theta_3
 *            = Sum_{k} q^(k^2), Delta_8 = q*Prod_{m>=1}
 *            (1-q^(2m-1))^8*(1-q^(4m))^8 = theta_2^4*theta_4^4/16, and
 *            Delta_8 = Sum_m (-1)^(m+1) W(m) q^m with W = (Sum_{j>=0}
 *            sigma(2j+1) q^(2j+1)) * theta_3^4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_zero, mul_theta, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/mul_theta.h"

static seq_t *T8 = 0, *T15 = 0, *W = 0;
static idx_t cap = 0;

static void build(idx_t len)
{
    idx_t i, d, m;
    if (cap) {
        release(T8, cap);
        release(T15, cap);
        release(W, cap);
    }
    T8 = alloc_zero(len);
    T15 = alloc_zero(len);
    W = alloc_zero(len);

    T8[0] = 1;
    for (i = 0; i < 8; i++) mul_theta(T8, len);
    for (i = 0; i < len; i++) T15[i] = T8[i];
    for (i = 0; i < 7; i++) mul_theta(T15, len);

    /* W starts as theta_2^4/16 = Sum sigma(2j+1) q^(2j+1) */
    for (d = 1; d < len; d += 2)
        for (m = d; m < len; m += 2 * d)
            W[m] += (seq_t)d;
    for (i = 0; i < 4; i++) mul_theta(W, len);

    cap = len;
}

seq_t a(idx_t n)
{
    if (n >= cap) {
        idx_t len = cap * 2;
        if (len < n + 1) len = n + 1;
        if (len < 64) len = 64;
        build(len);
    }
    seq_t pos = 0, odd = 0, even = 0;
    for (idx_t i = 0; i <= n; i++)
        pos += T15[i] * T8[n - i];
    for (idx_t m = 1; m <= n; m++) {
        seq_t t = T15[n - m] * W[m];
        if (m & 1) odd += t;
        else even += t;
    }
    pos += even * 46;
    return pos - odd * 46;
}
