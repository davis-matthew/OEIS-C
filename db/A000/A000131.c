/*
 * A000131  Number of asymmetrical dissections of n-gon.
 * https://oeis.org/A000131
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (C(n-2) - (n/2)*C(n/2-1) - n*C(floor(n/2)-1) -
 *            (n/3)*C(n/3-1) + n*C(n/4-1) + n*C(n/6-1)) / (2*n), where C =
 *            Catalan (A000108) and C(x) = 0 for noninteger x
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 24, u64 n <= 41, u128 n <= 74,
 *            big any n
 * Helpers:   bn_add, bn_addcat, bn_catalan, bn_div_small, bn_mul_small,
 *            bn_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bn_add.h"
#include "../utils/bn_addcat.h"
#include "../utils/bn_catalan.h"
#include "../utils/bn_div_small.h"
#include "../utils/bn_mul_small.h"
#include "../utils/bn_sub.h"

seq_t a(idx_t n)
{
    size_t L = (size_t)((2 * n) / 32 + 4);
    uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    seq_t res = 0;
    size_t j;

    /* positive part */
    bn_addcat(P, T, L, n - 2, 1);
    if (n % 4 == 0) bn_addcat(P, T, L, n / 4 - 1, n);
    if (n % 6 == 0) bn_addcat(P, T, L, n / 6 - 1, n);

    /* negative part */
    if (n % 2 == 0) bn_addcat(N, T, L, n / 2 - 1, n / 2);
    bn_addcat(N, T, L, n / 2 - 1, n);
    if (n % 3 == 0) bn_addcat(N, T, L, n / 3 - 1, n / 3);

    bn_sub(P, N, L);
    bn_div_small(P, L, 2 * n);

    j = L;
    while (j > 0 && P[j - 1] == 0) j--;
    while (j > 0) {
        j--;
        res = res * 65536;
        res = res * 65536;
        res += (seq_t)P[j];
    }

    free(P);
    free(N);
    free(T);
    return res;
}
