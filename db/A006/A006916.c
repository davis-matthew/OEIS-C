/*
 * A006916  Theta series of laminated lattice LAMBDA_13^{mid}.
 * https://oeis.org/A006916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   with y = theta_3(q^2) = Sum_{k in Z} q^(2k^2) and t =
 *            (theta_3(q)^2 - theta_3(q^2)^2)/4 = q*psi(q^4)^2, where psi(q) =
 *            Sum_{k>=0} q^(k(k+1)/2), a(n) = [q^n] ( y^13 + 864*y^9*t^2 +
 *            6400*y^7*t^3 + 20736*y^5*t^4 + 20480*y^3*t^5 )
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 33, u64 n <= 1885,
 *            u128 n <= 9242, big any n
 * Helpers:   add_scaled, alloc_zero, mul_p, mul_t, mul_y, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/add_scaled.h"
#include "../utils/mul_p.h"
#include "../utils/mul_t.h"
#include "../utils/mul_y.h"

static seq_t *cache = NULL;
static long long cache_len = 0;

static void build(long long L)
{
    seq_t *R = alloc_zero((idx_t)L);
    seq_t *Y = alloc_zero((idx_t)L);
    R[0] = 20480;
    mul_t(R, L);
    Y[0] = 1;
    mul_y(Y, L); mul_y(Y, L);              /* y^2 */
    add_scaled(R, Y, (seq_t)20736, L);
    mul_t(R, L);
    mul_y(Y, L); mul_y(Y, L);              /* y^4 */
    add_scaled(R, Y, (seq_t)6400, L);
    mul_t(R, L);
    mul_y(Y, L); mul_y(Y, L);              /* y^6 */
    add_scaled(R, Y, (seq_t)864, L);
    mul_t(R, L);
    mul_t(R, L);
    mul_y(Y, L); mul_y(Y, L);
    mul_y(Y, L); mul_y(Y, L);              /* y^10 */
    add_scaled(R, Y, (seq_t)1, L);
    mul_y(R, L); mul_y(R, L); mul_y(R, L); /* times y^3 */
    release(Y, (idx_t)L);
    if (cache) release(cache, (idx_t)cache_len);
    cache = R;
    cache_len = L;
}

seq_t a(idx_t n)
{
    long long need = (long long)n + 1;
    if (need > cache_len) {
        long long L = cache_len * 2;
        if (L < need) L = need;
        if (L < 64) L = 64;
        build(L);
    }
    return cache[(long long)n];
}
