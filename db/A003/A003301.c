/*
 * A003301  Numerators of coefficients of Green function for cubic lattice.
 * https://oeis.org/A003301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of A002893(n) * 4^n / (9^n * (2n+1) *
 *            binomial(2n,n)), where A002893(n) = Sum_{k=0..n}
 *            binomial(n,k)^2*binomial(2k,k), equivalently (n+1)^2*u(n+1) =
 *            (10n^2+10n+3)*u(n) - 9n^2*u(n-1), u(0) = 1, u(1) = 3. This
 *            solves 9(n+1)(2n+1)(2n+3)c(n+1) - 2(2n+1)(10n^2+10n+3)c(n) +
 *            4n^3 c(n-1)...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   a3301_big, a3301_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a3301_big.h"
#include "../utils/a3301_u.h"

seq_t a(idx_t n) {
    seq_t u = a3301_u(n);
    idx_t N = 2 * n + 1;
    idx_t i, j, p;
    unsigned char *comp = (unsigned char *)calloc((size_t)N + 1, 1);
    idx_t e2, pc = 0, t;

    for (i = 2; i * i <= N; i++)
        if (!comp[i])
            for (j = i * i; j <= N; j += i) comp[j] = 1;

    for (p = 3; p <= N; p += 2) {
        idx_t e = 0, q, x;
        if (comp[p]) continue;
        if (p == 3) e += 2 * n;
        x = N;
        while (x % p == 0) { e++; x /= p; }
        for (q = p; q <= 2 * n; ) {
            e += (2 * n) / q - 2 * (n / q);
            if (q > (2 * n) / p) break;
            q *= p;
        }
        while (e > 0 && u % p == 0) {
            u /= p;
            e--;
        }
    }
    free(comp);

    t = n;
    while (t) { pc += t & 1; t >>= 1; }
    e2 = 2 * n - pc;
    while (e2 >= 10) { u *= 1024; e2 -= 10; }
    while (e2 > 0) { u *= 2; e2--; }
    return u;
}
