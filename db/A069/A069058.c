/*
 * A069058  Numbers n such that A017666(n)=phi(n).
 * https://oeis.org/A069058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th number m = 2^i*3^j (3-smooth, in increasing order)
 *            with m/gcd(m,sigma(m)) = phi(m); exponents tested via LTE,
 *            enumerated by Hamming merge
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 53, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 81 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool A069058_ok(idx_t i, idx_t j)
{
    /* v2 of sigma(3^j) = (3^k-1)/2, k=j+1 */
    idx_t k = j + 1, a = 0, b = 0;
    if (k % 2 == 0) {
        idx_t t = k;
        a = 1;
        while (t % 2 == 0) { a++; t /= 2; }
    }
    /* v3 of sigma(2^i) = 2^(i+1)-1 */
    k = i + 1;
    if (k % 2 == 0) {
        idx_t t = k / 2;
        b = 1;
        while (t % 3 == 0) { b++; t /= 3; }
    }
    if (a > i) a = i;
    if (b > j) b = j;
    idx_t e2 = (i > 0) ? i - 1 : 0;
    idx_t e3 = 0;
    if (j > 0) { e2 += 1; e3 = j - 1; }
    return (i - a == e2) && (j - b == e3);
}

seq_t a(idx_t n)
{
    const double L2 = 0.69314718055994530942, L3 = 1.09861228866810969140;
    size_t cap = 1024, k = 1, idx = 0;
    idx_t *e2 = (idx_t *)malloc(cap * sizeof(idx_t));
    idx_t *e3 = (idx_t *)malloc(cap * sizeof(idx_t));
    size_t i2 = 0, i3 = 0;
    idx_t count = 0, ri = 0, rj = 0;
    e2[0] = 0; e3[0] = 0;
    for (;;) {
        if (A069058_ok(e2[idx], e3[idx])) {
            count++;
            if (count == n) { ri = e2[idx]; rj = e3[idx]; break; }
        }
        idx++;
        if (idx >= k) {
            if (k >= cap) {
                cap *= 2;
                e2 = (idx_t *)realloc(e2, cap * sizeof(idx_t));
                e3 = (idx_t *)realloc(e3, cap * sizeof(idx_t));
            }
            idx_t a2 = e2[i2] + 1, b2 = e3[i2];
            idx_t a3 = e2[i3], b3 = e3[i3] + 1;
            if (a2 == a3 && b2 == b3) {
                e2[k] = a2; e3[k] = b2; i2++; i3++;
            } else {
                double v2 = (double)a2 * L2 + (double)b2 * L3;
                double v3 = (double)a3 * L2 + (double)b3 * L3;
                if (v2 < v3) { e2[k] = a2; e3[k] = b2; i2++; }
                else { e2[k] = a3; e3[k] = b3; i3++; }
            }
            k++;
        }
    }
    free(e2);
    free(e3);
    seq_t r = 1;
    for (idx_t t = 0; t < ri; t++) r *= 2;
    for (idx_t t = 0; t < rj; t++) r *= 3;
    return r;
}
