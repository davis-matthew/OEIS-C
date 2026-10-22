/*
 * A005800  Generalized Euler numbers of type 3^2n.
 * https://oeis.org/A005800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/36^n) * Sum_{i=0..2*n} binomial(2*n, i) *
 *            A000364(n+i), where A000364(m) = E_{2m} = A000111(2m) is
 *            computed with the Entringer (Seidel) triangle E(k,0) = 0 for
 *            k>0, E(0,0) = 1, E(k,j) = E(k,j-1) + E(k-1,k-j). All of the
 *            arithmetic uses exact multiprecision integers made of 32-bit
 *            limbs
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   a5800_add, a5800_divsmall, a5800_nbits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a5800_add.h"
#include "../utils/a5800_divsmall.h"
#include "../utils/a5800_nbits.h"

seq_t a(idx_t n) {
    uint64_t K = 6 * (uint64_t)n;
    size_t L = (size_t)((K * a5800_nbits(K) + 2 * n) / 32 + 4);
    size_t cnt = (size_t)(2 * n + 1);
    uint32_t *prev = (uint32_t *)calloc((size_t)(K + 1) * L, sizeof(uint32_t));
    uint32_t *curr = (uint32_t *)calloc((size_t)(K + 1) * L, sizeof(uint32_t));
    uint32_t *b = (uint32_t *)calloc(cnt * L, sizeof(uint32_t));
    uint64_t k, j, p;
    seq_t r = 0;
    size_t top, i;

    /* row 0 */
    prev[0] = 1;
    if (n == 0) b[0] = 1;

    for (k = 1; k <= K; k++) {
        size_t Lk = (size_t)((k * a5800_nbits(k)) / 32 + 2);
        uint32_t *tmp;
        if (Lk > L) Lk = L;
        memset(curr, 0, L * sizeof(uint32_t));
        for (j = 1; j <= k; j++) {
            uint32_t *dst = curr + (size_t)j * L;
            memcpy(dst, curr + (size_t)(j - 1) * L, Lk * sizeof(uint32_t));
            a5800_add(dst, prev + (size_t)(k - j) * L, Lk);
        }
        if (k % 2 == 0 && k / 2 >= n) {
            uint64_t m = k / 2 - n;
            memcpy(b + (size_t)m * L, curr + (size_t)k * L, Lk * sizeof(uint32_t));
        }
        tmp = prev; prev = curr; curr = tmp;
    }

    /* binomial transform: sum_i C(2n,i) b_i */
    for (p = 0; p < 2 * n; p++) {
        for (j = 0; j + 1 + p <= 2 * n; j++) {
            a5800_add(b + (size_t)j * L, b + (size_t)(j + 1) * L, L);
        }
    }

    for (p = 0; p < n; p++) a5800_divsmall(b, L, 36);

    top = L;
    while (top > 0 && b[top - 1] == 0) top--;
    for (i = top; i-- > 0;) {
        r *= 256; r *= 256; r *= 256; r *= 256;
        r += (idx_t)b[i];
    }

    free(prev);
    free(curr);
    free(b);
    return r;
}
