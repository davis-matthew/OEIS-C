/*
 * A124634  Poincaré series [or Poincare series] P(C^o_{4,2}; x).
 * https://oeis.org/A124634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   multiply_poly
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A124634 – Poincaré series P(C^o_{4,2}; x)
    The generating function is G(x) = N(x) / D(x) where

    N(x) = 1 + 4x² + 12x³ + 36x⁴ + 68x⁵ + 171x⁶ + 316x⁷ + 639x⁸
           + 2192x⁹ + 1849x¹⁰ + 2794x¹¹ + 4151x¹² + 5546x¹³ + 7229x¹⁴
           + 8700x¹⁵ + 10085x¹⁶ + 10836x¹⁷ + 11270x¹⁸ + 10836x¹⁹
           + 10085x²⁰ + 8700x²¹ + 7229x²² + 5546x²³ + 4151x²⁴
           + 2794x²⁵ + 1849x²⁶ + 2192x²⁷ + 639x²⁸ + 316x²⁹ + 171x³⁰
           + 68x³¹ + 36x³² + 12x³³ + 4x³⁴ + x³⁶ + 316x⁷

    D(x) = (1-x)²(1-x²)³(1-x³)⁴(1-x⁴)⁴(1-x⁵)²(1-x⁶)

    Expanding D(x) gives a polynomial ∑_{i=0}^{52} D_i x^i
    (D_0 = 1).  The coefficients a(n) satisfy the linear recurrence

           a(n) = N_n - ∑_{i=1}^{52} D_i a(n-i)

    with a(0) = 1 and N_n = 0 for n > 36.  For n < 52 terms with
    a(n-i) that do not exist are treated as zero.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/multiply_poly.h"

static const int64_t Ncoeff[37] = {
    1, 0, 4, 12, 36, 68, 171, 316, 639, 2192,
    1849, 2794, 4151, 5546, 7229, 8700, 10085,
    10836, 11270, 10836, 10085, 8700, 7229,
    5546, 4151, 2794, 1849, 2192, 639,
    316, 171, 68, 36, 12, 4, 0, 1
};

static int64_t Dcoeff[53];  // coefficients of D(x)
static bool D_initialized = false;

static void init_Dcoeff(void)
{
    if (D_initialized) return;

    int64_t cur[53] = {0};
    cur[0] = 1;
    int curdeg = 0;

    const int64_t f1[3]  = {1, -2, 1};                                            /* (1-x)^2 */
    const int64_t f2[7]  = {1, 0, -3, 0, 3, 0, -1};                               /* (1-x^2)^3 */
    const int64_t f3[13] = {1, 0, 0, -4, 0, 0, 6, 0, 0, -4, 0, 0, 1};           /* (1-x^3)^4 */
    const int64_t f4[17] = {1, 0, 0, 0, -4, 0, 0, 0, 6, 0, 0, 0, -4, 0, 0, 0, 1};/* (1-x^4)^4 */
    const int64_t f5[11] = {1, 0, 0, 0, 0, -2, 0, 0, 0, 0, 1};                      /* (1-x^5)^2 */
    const int64_t f6[7]  = {1, 0, 0, 0, 0, 0, -1};                                 /* (1-x^6)  */

    multiply_poly(cur, &curdeg, f1, 2);
    multiply_poly(cur, &curdeg, f2, 6);
    multiply_poly(cur, &curdeg, f3, 12);
    multiply_poly(cur, &curdeg, f4, 16);
    multiply_poly(cur, &curdeg, f5, 10);
    multiply_poly(cur, &curdeg, f6, 6);

    for (int i = 0; i <= 52; ++i)
        Dcoeff[i] = cur[i];

    D_initialized = true;
}

seq_t a(idx_t n)
{
    init_Dcoeff();               // ensure D_coeffs are ready

    if (n == 0)
        return (seq_t)1;

    /* allocate array for terms 0..n (n+1 elements) */
    size_t sz = (size_t)n + 1;
    seq_t *arr = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!arr)
        abort();

    arr[0] = (seq_t)1;           /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t result = (seq_t)(i <= 36 ? Ncoeff[(int)i] : 0);

        for (int k = 1; k <= 52 && k <= (int)i; ++k) {
            int64_t coeff = Dcoeff[k];
            if (coeff == 0) continue;

            seq_t prod = arr[i - k] * (seq_t)llabs(coeff);
            if (coeff < 0)
                result += prod;   /* add positive contribution */
            else
                result -= prod;   /* subtract positive contribution */
        }
        arr[i] = result;
    }

    seq_t ret = arr[n];
    free(arr);
    return ret;
}
