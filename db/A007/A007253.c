/*
 * A007253  McKay-Thompson series of class 5a for Monster.
 * https://oeis.org/A007253
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   with y = eta(q)/eta(q^25) + 1 (= 1/s - s, where s is the
 *            Rogers-Ramanujan continued fraction R(q^5)), the g.f. is T5a =
 *            (y^6 + 20*y^3 + 84*y + 20) / (y^5 + 5*y^3 + 5*y - 11) = y +
 *            (-5*y^4 + 20*y^3 - 5*y^2 + 95*y + 20) * q^5 * Prod_{k>=1}
 *            (1-q^(25k))^6/(1-q^(5k))^6. It is computed as power series,...
 * Range:     i8 n <= 6, i16 n <= 26, i32 n <= 101, i64 n <= 366,
 *            i128 n <= 1000, big any n
 * Helpers:   alloc_zero, release, t5a_div_euler, t5a_mulY, t5a_mul_euler
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/t5a_div_euler.h"
#include "../utils/t5a_mulY.h"
#include "../utils/t5a_mul_euler.h"

static seq_t *t5a_tab = 0;
static long long t5a_len = 0;

static void t5a_build(long long K)
{
    long long i;
    seq_t *Y = alloc_zero(K);
    seq_t *B = alloc_zero(K);
    seq_t *tmp = alloc_zero(K);
    seq_t *res = alloc_zero(K);

    /* Y = q*y = E1/E25 + q */
    Y[0] = 1;
    t5a_mul_euler(Y, K, 1);
    t5a_div_euler(Y, K, 25);
    if (K > 1) Y[1] += 1;

    /* Horner: B = (((-5Y + 20q)Y - 5q^2)Y + 95q^3)Y + 20q^4 */
    for (i = 0; i < K; i++) B[i] = Y[i] * (-5);
    if (K > 1) B[1] += 20;
    t5a_mulY(B, tmp, K);
    if (K > 2) B[2] -= 5;
    t5a_mulY(B, tmp, K);
    if (K > 3) B[3] += 95;
    t5a_mulY(B, tmp, K);
    if (K > 4) B[4] += 20;

    /* C = q^2 * B */
    for (i = K - 1; i >= 2; i--) res[i] = B[i - 2];

    /* C *= E25^6 / E5^6 */
    for (i = 0; i < 6; i++) t5a_mul_euler(res, K, 25);
    for (i = 0; i < 6; i++) t5a_div_euler(res, K, 5);

    /* q*T = Y + C */
    for (i = 0; i < K; i++) res[i] += Y[i];

    release(Y, K);
    release(B, K);
    release(tmp, K);
    if (t5a_tab) release(t5a_tab, t5a_len);
    t5a_tab = res;
    t5a_len = K;
}

seq_t a(idx_t n)
{
    long long need = (long long)n + 2;
    if (need > t5a_len) {
        long long K = 2 * t5a_len;
        if (K < need) K = need;
        if (K < 64) K = 64;
        t5a_build(K);
    }
    return t5a_tab[n + 1];
}
