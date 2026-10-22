/*
 * A001332  a(n) = Bernoulli(2*n) * (2*n + 1)!.
 * https://oeis.org/A001332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(0) = 1; for n >= 1, a(n) = (-1)^(n-1) * 2n * (2n+1)! * T(n) /
 *            (4^n * (4^n - 1)), where T(n) = A000182(n) are the tangent
 *            numbers (from B(2n) = (-1)^(n-1)*2n*T(n)/(4^n*(4^n-1))). T(n) is
 *            computed with the integer recurrence: T[k] = (k-1)! at the
 *            start, then for k = 2..N, j = k..N: T[j] = (j-k)*T...
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   a1332_gcd, alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/a1332_gcd.h"

static seq_t *a1332_T = NULL;   /* a1332_T[1..a1332_cap] = tangent numbers */
static idx_t a1332_cap = 0;

static void a1332_tangent(idx_t N) {
    if (N <= a1332_cap) return;
    if (N > 64 && N < 2 * a1332_cap) N = 2 * a1332_cap;
    seq_t *T = alloc_zero(N + 1);
    T[1] = 1;
    for (idx_t k = 2; k <= N; k++)
        T[k] = T[k - 1] * (seq_t)(long long)(k - 1);
    for (idx_t k = 2; k <= N; k++)
        for (idx_t j = k; j <= N; j++)
            T[j] = T[j - 1] * (seq_t)(long long)(j - k) + T[j] * (seq_t)(long long)(j - k + 2);
    if (a1332_T != NULL) release(a1332_T, a1332_cap + 1);
    a1332_T = T;
    a1332_cap = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    a1332_tangent(n);
    seq_t x = a1332_T[n];

    if (n <= 31) {
        /* Cancel the denominator 2^(2n) * (4^n - 1) against the small factors first,
           so intermediates never exceed |a(n)|. */
        uint64_t D = (((uint64_t)1) << (2 * n)) - 1;
        idx_t e2 = 2 * n;
        uint64_t f[64];
        int nf = 0;
        for (idx_t i = 1; i <= 2 * n + 2; i++) {
            uint64_t v = (i == 2 * n + 2) ? (uint64_t)(2 * n) : (uint64_t)i;
            while (e2 > 0 && v % 2 == 0) {
                v /= 2;
                e2--;
            }
            for (;;) {
                uint64_t g = a1332_gcd(v, D);
                if (g <= 1) break;
                v /= g;
                D /= g;
            }
            f[nf++] = v;
        }
        /* Remaining 2^e2 and D are coprime to the reduced factors, hence divide T(n). */
        while (e2 > 0) {
            x /= 2;
            e2--;
        }
        if (D > 1) {
            seq_t d = (seq_t)(long long)D;
            if (d <= 0) return 0;  /* cannot occur when a(n) fits in seq_t */
            x /= d;
        }
        for (int i = 0; i < nf; i++)
            if (f[i] > 1) x *= (seq_t)(long long)f[i];
    } else {
        seq_t num = x * (seq_t)(long long)(2 * n);
        for (idx_t i = 2; i <= 2 * n + 1; i++)
            num *= (seq_t)(long long)i;
        seq_t p = 1;
        for (idx_t i = 0; i < n; i++)
            p *= 4;
        x = num / (p * (p - 1));
    }

    if (n % 2 == 0) x = -x;
    return x;
}
