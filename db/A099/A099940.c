/*
 * A099940  a(n) = 2*(A056855(n)) /(phi(n)*n), where phi() is the Euler phi
 *          function.
 * https://oeis.org/A099940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A056855(n)/(phi(n)*n), where A056855(n) = (Product
 *            k)*(Sum 1/k) over k <= n with gcd(k,n) = 1. Computed as
 *            2*(P/D)*N/(phi(n)*n), where N/D is the reduced harmonic-type
 *            sum. Common factors are cancelled with gcd before multiplying,
 *            so no intermediate value overflows
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* numerator factors: 2, every k coprime to n, and N */
    seq_t *f = mem_new(seq_t, (size_t)n + 3);
    size_t cnt = 0;
    f[cnt++] = 2;

    seq_t num = 0, den = 1;   /* running reduced fraction sum of 1/k */
    seq_t phi = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t K = (seq_t)k;
        if (gcd(K, (seq_t)n) != 1) continue;
        phi++;
        f[cnt++] = K;
        seq_t g = gcd(den, K);
        seq_t dg = den / g;
        seq_t kg = K / g;
        num = num * kg + dg;
        den = dg * K;
        seq_t h = gcd(num, den);
        if (h != 1) { num /= h; den /= h; }
    }
    f[cnt++] = num;

    /* denominators: D (den), phi, n */
    seq_t dens[3];
    dens[0] = den;
    dens[1] = phi;
    dens[2] = (seq_t)n;
    for (int j = 0; j < 3; ++j) {
        seq_t d = dens[j];
        for (size_t i = 0; i < cnt && d != 1; ++i) {
            seq_t g = gcd(f[i], d);
            if (g != 1) {
                f[i] /= g;
                d /= g;
            }
        }
    }

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) res *= f[i];
    mem_free(f);
    return res;
}
