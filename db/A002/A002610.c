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
 * Helpers:   a1384_zeros, a2610_mul_euler
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a1384_zeros.h"
#include "../utils/a2610_mul_euler.h"

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
