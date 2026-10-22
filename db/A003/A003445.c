/*
 * A003445  Number of nonequivalent dissections of an n-gon into n-4 polygons
 *          by nonintersecting diagonals up to rotation.
 * https://oeis.org/A003445
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( D(n) + [n even]*(n/2)*C(n-3, n/2-3) +
 *            [5|n]*4*(n/5)*Catalan(n/5-1) ) / n, where D(n) = (n-3)*C(2n-6,
 *            n-5)/2 is the number of dissections of an n-gon with n-5
 *            diagonals (Burnside's lemma over the cyclic group; only
 *            rotations of order 2 and 5 fix such a dissection)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Helpers:   a3445_add, a3445_div, a3445_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3445_add.h"
#include "../utils/a3445_div.h"
#include "../utils/a3445_mul.h"

seq_t a(idx_t n)
{
    size_t cap = (size_t)((4 * (uint64_t)n + 512) / 32);
    uint32_t *S = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t sl = 1, tl, k;
    uint64_t N = (uint64_t)n - 3, i;
    seq_t res = 0;

    /* D = (n-3) * C(2N, N-2) / 2, N = n-3 */
    S[0] = 1;
    for (i = 1; i + 2 <= N; i++) {
        a3445_mul(S, &sl, N + 2 + i);
        a3445_div(S, &sl, i);
    }
    a3445_mul(S, &sl, N);
    a3445_div(S, &sl, 2);

    if (n % 2 == 0) {
        uint64_t h = (uint64_t)n / 2;
        memset(T, 0, cap * sizeof(uint32_t));
        T[0] = 1; tl = 1;
        for (i = 1; i + 3 <= h; i++) {
            a3445_mul(T, &tl, h + i);
            a3445_div(T, &tl, i);
        }
        a3445_mul(T, &tl, h);
        a3445_add(S, &sl, T, tl);
    }
    if (n % 5 == 0) {
        uint64_t m = (uint64_t)n / 5, j = m - 1;
        memset(T, 0, cap * sizeof(uint32_t));
        T[0] = 1; tl = 1;
        for (i = 1; i <= j; i++) {
            a3445_mul(T, &tl, j + i);
            a3445_div(T, &tl, i);
        }
        a3445_div(T, &tl, j + 1);
        a3445_mul(T, &tl, 4 * m);
        a3445_add(S, &sl, T, tl);
    }
    a3445_div(S, &sl, (uint64_t)n);

    k = sl;
    while (k > 0) {
        k--;
        res = (seq_t)(res << 16);
        res = (seq_t)(res << 16);
        res += (seq_t)S[k];
    }
    free(S);
    free(T);
    return res;
}
