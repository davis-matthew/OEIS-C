/*
 * A002678  Numerators of the Taylor coefficients of (e^x-1)^2.
 * https://oeis.org/A002678
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (2^n-2)/n! = (2^(n-1)-1) / gcd(2^(n-1)-1,
 *            n!)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   q2678_div, q2678_mod, q2678_pow2mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/q2678_div.h"
#include "../utils/q2678_mod.h"
#include "../utils/q2678_pow2mod.h"

seq_t a(idx_t n) {
    idx_t m = n - 1;
    size_t L = (size_t)(m / 32) + 1;
    uint32_t *x = (uint32_t *)calloc(L, sizeof(uint32_t));
    for (idx_t i = 0; i < m; i++)
        x[i / 32] |= (uint32_t)1 << (i % 32);

    unsigned char *comp = (unsigned char *)calloc((size_t)n + 1, 1);
    for (idx_t i = 2; i * i <= n; i++)
        if (!comp[i])
            for (idx_t j = i * i; j <= n; j += i) comp[j] = 1;

    for (idx_t p = 3; p <= n; p += 2) {
        if (comp[p]) continue;
        if (q2678_pow2mod(m, p) != 1) continue;
        idx_t e = 0;
        for (idx_t t = n / p; t > 0; t /= p) e += t;
        while (e > 0) {
            if (q2678_mod(x, L, (uint32_t)p) != 0) break;
            q2678_div(x, L, (uint32_t)p);
            e--;
        }
    }

    seq_t r = (seq_t)0;
    for (size_t i = L; i-- > 0;) {
        int s;
        for (s = 24; s >= 0; s -= 8) {
            r = (seq_t)(r << 8);
            r = r + (seq_t)((x[i] >> s) & 255u);
        }
    }
    free(x);
    free(comp);
    return r;
}
