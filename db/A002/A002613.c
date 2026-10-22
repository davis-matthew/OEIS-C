/*
 * A002613  Glaisher's function J(n) (18 squares version).
 * https://oeis.org/A002613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [q^n] theta3(q)^2 * F(q) * (theta3(q)^4 + 16*F(q))^2 *
 *            theta4(q)^4, where theta3(q) = Sum_{k} q^(k^2), theta4(q) =
 *            theta3(-q), F(q) = Sum_{k>=0} sigma(2k+1) q^(2k+1) =
 *            q*psi(q^2)^4, psi(x) = Sum_{k>=0} x^(k(k+1)/2). Equivalently
 *            a(n) = [q^n] (theta^14*F + 16*theta^10*F^2 - 256*theta^6*F^3...
 * Range:     i8 n <= 2, i16 n <= 10, i32 n <= 147, i64 n <= 500,
 *            i128 n <= 500, big any n
 * Helpers:   from_i128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/from_i128.h"

typedef unsigned __int128 a2613_u128;

static a2613_u128 *a2613_cache = NULL;
static long long a2613_cap = -1;

/* a := a * theta(sgn*q), in place (arithmetic mod 2^128) */
static void a2613_mul_theta(a2613_u128 *a, long long N, int sgn)
{
    long long i, k;
    for (i = N; i >= 0; i--) {
        a2613_u128 s = a[i];
        for (k = 1; k * k <= i; k++) {
            a2613_u128 t = a[i - k * k];
            if (sgn < 0 && (k & 1))
                s -= 2 * t;
            else
                s += 2 * t;
        }
        a[i] = s;
    }
}

/* a := a * psi(q^2) = a * Sum_{k>=0} q^(k(k+1)) */
static void a2613_mul_psi2(a2613_u128 *a, long long N)
{
    long long i, k;
    for (i = N; i >= 0; i--) {
        a2613_u128 s = a[i];
        for (k = 1; k * (k + 1) <= i; k++)
            s += a[i - k * (k + 1)];
        a[i] = s;
    }
}

/* a := a * F = a * q * psi(q^2)^4 */
static void a2613_mul_F(a2613_u128 *a, long long N)
{
    long long i;
    int j;
    for (i = N; i >= 1; i--)
        a[i] = a[i - 1];
    a[0] = 0;
    for (j = 0; j < 4; j++)
        a2613_mul_psi2(a, N);
}

/* a := a * (theta^4 + 16 F) */
static void a2613_mul_P(a2613_u128 *a, a2613_u128 *b, long long N)
{
    long long i;
    int j;
    memcpy(b, a, (size_t)(N + 1) * sizeof(a2613_u128));
    for (j = 0; j < 4; j++)
        a2613_mul_theta(a, N, 1);
    a2613_mul_F(b, N);
    for (i = 0; i <= N; i++)
        a[i] += 16 * b[i];
}

static void a2613_build(long long N)
{
    a2613_u128 *a = (a2613_u128 *)malloc((size_t)(N + 1) * sizeof(a2613_u128));
    a2613_u128 *b = (a2613_u128 *)malloc((size_t)(N + 1) * sizeof(a2613_u128));
    long long i;
    int j;
    for (i = 0; i <= N; i++)
        a[i] = 0;
    a[0] = 1;
    a2613_mul_theta(a, N, 1);
    a2613_mul_theta(a, N, 1);
    a2613_mul_F(a, N);
    a2613_mul_P(a, b, N);
    a2613_mul_P(a, b, N);
    for (j = 0; j < 4; j++)
        a2613_mul_theta(a, N, -1);
    free(b);
    if (a2613_cache)
        free(a2613_cache);
    a2613_cache = a;
    a2613_cap = N;
}

seq_t a(idx_t n)
{
    if ((long long)n > a2613_cap) {
        long long N = 2 * (long long)n;
        if (N < 1024)
            N = 1024;
        a2613_build(N);
    }
    return from_i128((__int128)a2613_cache[n]);
}
