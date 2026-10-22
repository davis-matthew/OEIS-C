/*
 * A002392  Decimal expansion of natural logarithm of 10.
 * https://oeis.org/A002392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = n-th digit of log(10), where log(10) = 46*atanh(1/31) +
 *            34*atanh(1/49) + 20*atanh(1/161), atanh(1/x) = Sum_{k>=0}
 *            1/((2k+1)*x^(2k+1)); evaluated in base-10^9 fixed point
 * Range:     u8 n <= 9085, u16 n <= 9085, u32 n <= 9085, u64 n <= 9085,
 *            u128 n <= 9085, big any n
 * Helpers:   ln10_add, ln10_divs, ln10_iszero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ln10_add.h"
#include "../utils/ln10_divs.h"
#include "../utils/ln10_iszero.h"

static uint32_t *ln10_fx = NULL;
static size_t ln10_limbs = 0;

static void ln10_compute(size_t digits) {
    static const uint32_t coef[3] = {46, 34, 20};
    static const uint32_t xs[3] = {31, 49, 161};
    size_t L = digits / 9 + 5;
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)malloc(L * sizeof(uint32_t));
    uint32_t *U = (uint32_t *)malloc(L * sizeof(uint32_t));
    int j;
    for (j = 0; j < 3; j++) {
        uint32_t x = xs[j];
        uint32_t m = 1;
        memset(T, 0, L * sizeof(uint32_t));
        T[0] = coef[j];
        ln10_divs(T, L, x);
        while (!ln10_iszero(T, L)) {
            memcpy(U, T, L * sizeof(uint32_t));
            ln10_divs(U, L, m);
            ln10_add(S, U, L);
            ln10_divs(T, L, x * x);
            m += 2;
        }
    }
    free(T);
    free(U);
    free(ln10_fx);
    ln10_fx = S;
    ln10_limbs = L;
}

seq_t a(idx_t n) {
    uint64_t k, li, pos;
    uint32_t limb;
    int p;
    if (n < 1) return 0;
    if (n == 1) {
        if (ln10_fx == NULL) ln10_compute(64);
        return (seq_t)ln10_fx[0];
    }
    k = (uint64_t)n - 1; /* fractional digit index, >= 1 */
    li = 1 + (k - 1) / 9;
    pos = (k - 1) % 9;
    if (ln10_fx == NULL || li + 3 >= ln10_limbs) {
        size_t want = (size_t)(k * 2 + 64);
        ln10_compute(want);
    }
    limb = ln10_fx[li];
    for (p = 8; p > (int)pos; p--) limb /= 10;
    return (seq_t)(limb % 10);
}
