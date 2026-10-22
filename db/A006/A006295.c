/*
 * A006295  Number of genus 1 rooted maps with 2 faces with n vertices.
 * https://oeis.org/A006295
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. x*(1-sqrt(1-4x))*(11+12x+9*sqrt(1-4x))/(4*(4x-1)^4) =
 *            x*[(1+24x)*(1-4x)^(-4) - (1+6x)*(1-4x)^(-7/2)]/2, so with m =
 *            n-2: a(n) = (A(m) + 24*A(m-1) - B(m) - 6*B(m-1))/2, A(k) =
 *            4^k*C(k+3,3), B(k) = [x^k](1-4x)^(-7/2), B(0) = 1, B(k) =
 *            B(k-1)*2*(2k+5)/k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   a006295_g, a006295_mdiv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006295_g.h"
#include "../utils/a006295_mdiv.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    uint64_t m = n - 2;
    /* Bp = B(m-1) */
    seq_t Bp = 1;
    for (uint64_t k = 1; k + 1 <= m; k++)
        Bp = a006295_mdiv(Bp, 2 * (2 * k + 5), k);
    /* Bh = B(m)/2 */
    seq_t Bh = a006295_mdiv(Bp, 2 * m + 5, m);
    /* q = 4^(m-1) */
    seq_t q = 1;
    for (uint64_t k = 1; k < m; k++) q *= 4;
    uint64_t cm = (m + 1) * (m + 2) * (m + 3) / 6;
    uint64_t cm1 = m * (m + 1) * (m + 2) / 6;
    seq_t res = (seq_t)cm * q * 2;
    res += (seq_t)cm1 * q * 12;
    res -= Bh;
    res -= Bp * 3;
    return res;
}
