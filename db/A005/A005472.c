/*
 * A005472  Class numbers of Shanks' simplest cubic fields.
 * https://oeis.org/A005472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(K_m), the class number of the cyclic cubic field K_m =
 *            Q(rho), where rho is a root of x^3 - m*x^2 - (m+3)*x - 1, p =
 *            m^2+3m+9 is the n-th prime of that form (m >= -1), and disc(K_m)
 *            = p^2. Analytic class number formula: h*R = p*|L(1,chi)|^2/4,
 *            with chi a cubic character mod p. L(1,chi) i...
 * Range:     u8 n <= 70, u16 n <= 644, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9377 (2026-11-03)
 * Helpers:   is_prime_u64, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/powmod_u64.h"

static long double A005472_E1(long double x)
{
    int i;
    if (x <= 1.0L) {
        long double sum = -0.57721566490153286060651209L - logl(x);
        long double fact = 1.0L;
        for (i = 1; i < 500; i++) {
            long double del;
            fact *= -x / i;
            del = -fact / i;
            sum += del;
            if (fabsl(del) < fabsl(sum) * 1e-20L) break;
        }
        return sum;
    } else {
        long double b = x + 1.0L, c = 1e300L, d = 1.0L / b, h = d;
        for (i = 1; i < 2000; i++) {
            long double an = -(long double)i * i, del;
            b += 2.0L;
            d = 1.0L / (an * d + b);
            c = b + an / c;
            del = c * d;
            h *= del;
            if (fabsl(del - 1.0L) < 1e-19L) break;
        }
        return h * expl(-x);
    }
}

static unsigned long long A005472_classno(long long m)
{
    const long double PI = 3.14159265358979323846264338327950288L;
    const long double S3 = 0.86602540378443864676372317075293618L; /* sqrt(3)/2 */
    uint64_t p = (uint64_t)(m * m + 3 * m + 9);
    uint64_t e = (p - 1) / 3, g = 0, N, i, j;
    long double sp = sqrtl((long double)p);
    long double SA[2][3], SB[2][3];
    long double cs[2] = {1.0L, 2.0L};
    long double Are[2], Aim[2], Bre[2], Bim[2];
    long double nr, ni, dr, di, den, Wr, Wi, Lr, Li, hR, R;
    unsigned char *k;
    uint32_t *spf;
    int ci, t;

    N = (uint64_t)sqrtl(90.0L * (long double)p / PI) + 2;
    for (i = 2;; i++) {
        uint64_t v = powmod_u64(i, e, p);
        if (v != 1) { g = v; break; }
    }
    k = (unsigned char *)malloc((size_t)(N + 1));
    spf = (uint32_t *)calloc((size_t)(N + 1), sizeof(uint32_t));
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            for (j = i * i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    k[1] = 0;
    for (i = 2; i <= N; i++) {
        if (spf[i] == i) {
            if (i % p == 0) k[i] = 3;
            else {
                uint64_t v = powmod_u64(i % p, e, p);
                k[i] = (unsigned char)(v == 1 ? 0 : (v == g ? 1 : 2));
            }
        } else {
            unsigned char x = k[spf[i]], y = k[i / spf[i]];
            k[i] = (unsigned char)((x == 3 || y == 3) ? 3 : (x + y) % 3);
        }
    }
    for (ci = 0; ci < 2; ci++)
        for (t = 0; t < 3; t++) { SA[ci][t] = 0; SB[ci][t] = 0; }
    for (i = 1; i <= N; i++) {
        int kk = k[i];
        if (kk == 3) continue;
        for (ci = 0; ci < 2; ci++) {
            long double x1 = (long double)i * sqrtl(PI * cs[ci] / (long double)p);
            long double x2 = PI * (long double)i * (long double)i / ((long double)p * cs[ci]);
            if (x1 < 7.5L) SA[ci][kk] += erfcl(x1) / (long double)i;
            if (x2 < 46.0L) SB[ci][kk] += A005472_E1(x2);
        }
    }
    free(k);
    free(spf);
    for (ci = 0; ci < 2; ci++) {
        /* sum chi(n) f(n) = S0 + w S1 + w^2 S2 ; conj character for B */
        Are[ci] = SA[ci][0] - (SA[ci][1] + SA[ci][2]) / 2;
        Aim[ci] = S3 * (SA[ci][1] - SA[ci][2]);
        Bre[ci] = (SB[ci][0] - (SB[ci][1] + SB[ci][2]) / 2) / sp;
        Bim[ci] = (S3 * (SB[ci][2] - SB[ci][1])) / sp;
    }
    /* W = (A1 - A2) / (B2 - B1) */
    nr = Are[0] - Are[1]; ni = Aim[0] - Aim[1];
    dr = Bre[1] - Bre[0]; di = Bim[1] - Bim[0];
    den = dr * dr + di * di;
    Wr = (nr * dr + ni * di) / den;
    Wi = (ni * dr - nr * di) / den;
    {
        long double mod = sqrtl(Wr * Wr + Wi * Wi);
        Wr /= mod; Wi /= mod;
    }
    Lr = Are[0] + Wr * Bre[0] - Wi * Bim[0];
    Li = Aim[0] + Wr * Bim[0] + Wi * Bre[0];
    hR = (long double)p * (Lr * Lr + Li * Li) / 4.0L;

    /* regulator from the roots of x^3 - m x^2 - (m+3) x - 1 */
    {
        long double a = -(long double)m, b = -(long double)(m + 3), c = -1.0L;
        long double P = b - a * a / 3.0L;
        long double Q = 2.0L * a * a * a / 27.0L - a * b / 3.0L + c;
        long double r = 2.0L * sqrtl(-P / 3.0L);
        long double arg = (3.0L * Q / (2.0L * P)) * sqrtl(-3.0L / P);
        long double th, sum = 0;
        long double mm = (long double)m;
        if (arg > 1.0L) arg = 1.0L;
        if (arg < -1.0L) arg = -1.0L;
        th = acosl(arg) / 3.0L;
        for (t = 0; t < 3; t++) {
            long double x = r * cosl(th - 2.0L * PI * t / 3.0L) - a / 3.0L;
            int it;
            for (it = 0; it < 8; it++) {
                long double f = ((x - mm) * x - (mm + 3.0L)) * x - 1.0L;
                long double fp = (3.0L * x - 2.0L * mm) * x - (mm + 3.0L);
                if (fp == 0) break;
                x -= f / fp;
            }
            {
                long double lg = logl(fabsl(x));
                sum += lg * lg;
            }
        }
        R = sum / 2.0L;
    }
    return (unsigned long long)(hR / R + 0.5L);
}

static long long *A005472_ms = NULL;
static idx_t A005472_mcount = 0, A005472_mcap = 0;
static long long A005472_nextm = -1;

static long long A005472_get_m(idx_t n)
{
    while (A005472_mcount < n) {
        long long m = A005472_nextm++;
        uint64_t p = (uint64_t)(m * m + 3 * m + 9);
        if (is_prime_u64(p)) {
            if (A005472_mcount == A005472_mcap) {
                A005472_mcap = A005472_mcap ? 2 * A005472_mcap : 64;
                A005472_ms = (long long *)realloc(A005472_ms, A005472_mcap * sizeof(long long));
            }
            A005472_ms[A005472_mcount++] = m;
        }
    }
    return A005472_ms[n - 1];
}

seq_t a(idx_t n)
{
    long long m;
    if (n < 1) return 0;
    m = A005472_get_m(n);
    return (seq_t)A005472_classno(m);
}
