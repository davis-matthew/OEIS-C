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
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 44 terms through n = 9242 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static void add_scaled(seq_t *R, const seq_t *Y, seq_t c, long long L)
{
    long long i;
    for (i = 0; i < L; i++)
        R[i] += Y[i] * c;
}

/* A *= psi(q^4), in place */
static void mul_p(seq_t *A, long long L)
{
    long long i, k;
    for (i = L - 1; i >= 0; i--) {
        seq_t s = A[i];
        for (k = 1; 2 * k * (k + 1) <= i; k++)
            s += A[i - 2 * k * (k + 1)];
        A[i] = s;
    }
}

/* A *= t = q*psi(q^4)^2 */
static void mul_t(seq_t *A, long long L)
{
    long long i;
    mul_p(A, L);
    mul_p(A, L);
    for (i = L - 1; i >= 1; i--)
        A[i] = A[i - 1];
    A[0] = 0;
}

/* A *= theta_3(q^2), in place, truncated to length L */
static void mul_y(seq_t *A, long long L)
{
    long long i, k;
    for (i = L - 1; i >= 0; i--) {
        seq_t s = A[i];
        for (k = 1; 2 * k * k <= i; k++) {
            s += A[i - 2 * k * k];
            s += A[i - 2 * k * k];
        }
        A[i] = s;
    }
}

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
