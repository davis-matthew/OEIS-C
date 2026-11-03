/*
 * A002610  Glaisher's function H'(4n+1) (18 squares version).
 * https://oeis.org/A002610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^(n-1)] Product_{k>=1} (1-x^k)^6 * (1-x^(2k))^12, a(0)
 *            = 0 (i.e. q-expansion of eta(4z)^6 * eta(8z)^12 = Sum a(n)
 *            q^(4n+1))
 * Range:     i8 n <= 5, i16 n <= 19, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *a1384_zeros(idx_t len) {
    seq_t *p = mem_new(seq_t, len);
    idx_t i;
    for (i = 0; i < len; i++) p[i] = 0;
    return p;
}

/* multiply series f (length L) in place by Product_{k>=1} (1 - x^(s*k)) */
static void a2610_mul_euler(seq_t *f, idx_t L, idx_t s)
{
    idx_t i, k;
    for (i = L; i-- > 1; ) {
        seq_t acc = f[i];
        for (k = 1; ; k++) {
            idx_t p1 = s * (k * (3 * k - 1) / 2);
            idx_t p2 = s * (k * (3 * k + 1) / 2);
            if (p1 > i) break;
            if (k & 1) acc -= f[i - p1]; else acc += f[i - p1];
            if (p2 <= i) {
                if (k & 1) acc -= f[i - p2]; else acc += f[i - p2];
            }
        }
        f[i] = acc;
    }
}

static seq_t *A2610_F = 0;
static idx_t A2610_cap = 0;

seq_t a(idx_t n)
{
    idx_t m, L;
    int j;
    if (n == 0) return 0;
    m = n - 1;
    if (m >= A2610_cap) {
        L = 2 * A2610_cap;
        if (L < m + 1) L = m + 1;
        if (L < 1024) L = 1024;
        A2610_F = a1384_zeros(L);
        A2610_F[0] = 1;
        for (j = 0; j < 6; j++) a2610_mul_euler(A2610_F, L, 1);
        for (j = 0; j < 12; j++) a2610_mul_euler(A2610_F, L, 2);
        A2610_cap = L;
    }
    return A2610_F[m];
}
