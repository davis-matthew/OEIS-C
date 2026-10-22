/*
 * A003450  Number of nonequivalent dissections of an n-gon into n-4 polygons
 *          by nonintersecting diagonals up to rotation and reflection.
 * https://oeis.org/A003450
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside over the dihedral group D_n, with a(n) = ( D + R2 + R5
 *            + Refl ) / (2n), where D = (n-3)/2 * binomial(2n-6, n-5) (all
 *            (n-5)-diagonal dissections), R2 = [n even] (n/2) * binomial(n-3,
 *            n/2-3) (fixed by rotation of order 2), R5 = [5|n] 4 *
 *            binomial(2k-2, k-1), k = n/5 (fixed by rotations of...
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   a3450_add, a3450_ballot, a3450_binom, a3450_div, a3450_mul,
 *            a3450_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3450_add.h"
#include "../utils/a3450_ballot.h"
#include "../utils/a3450_binom.h"
#include "../utils/a3450_div.h"
#include "../utils/a3450_mul.h"
#include "../utils/a3450_width.h"

seq_t a(idx_t n) {
    long long N = (long long)n;
    size_t L = (size_t)((2 * N + 160) / 32 + 4);
    size_t i;
    int w = a3450_width();
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *U = (uint32_t *)calloc(L, sizeof(uint32_t));
    seq_t res = 0;

    /* identity */
    a3450_binom(T, L, 2 * N - 6, N - 5);
    a3450_mul(T, L, (uint32_t)(N - 3));
    a3450_div(T, L, 2);
    a3450_add(S, T, L);

    /* rotation by half-turn */
    if (N % 2 == 0 && N / 2 >= 3) {
        a3450_binom(T, L, N - 3, N / 2 - 3);
        a3450_mul(T, L, (uint32_t)(N / 2));
        a3450_add(S, T, L);
    }
    /* rotations of order 5 */
    if (N % 5 == 0) {
        long long k = N / 5;
        a3450_binom(T, L, 2 * k - 2, k - 1);
        a3450_mul(T, L, 4);
        a3450_add(S, T, L);
    }

    /* reflections */
    if (N % 2 == 1) {
        long long m = (N - 1) / 2;
        a3450_ballot(T, L, m, 2); a3450_add(U, T, L);
        a3450_ballot(T, L, m, 3); a3450_add(U, T, L);
        a3450_binom(T, L, 2 * m - 3, m - 3); a3450_add(U, T, L);
        a3450_mul(U, L, (uint32_t)N);
    } else {
        long long m = N / 2;
        a3450_binom(T, L, 2 * m - 3, m - 3); a3450_add(U, T, L);
        a3450_ballot(T, L, m, 3); a3450_mul(T, L, 2); a3450_add(U, T, L);
        a3450_ballot(T, L, m, 4); a3450_add(U, T, L);
        a3450_binom(T, L, 2 * m - 4, m - 4); a3450_mul(T, L, 2); a3450_add(U, T, L);
        a3450_ballot(T, L, m - 1, 2); a3450_add(U, T, L);
        a3450_mul(U, L, (uint32_t)m);
    }
    a3450_add(S, U, L);

    a3450_div(S, L, (uint32_t)(2 * N));

    for (i = 0; i < L; i++) {
        if (w < 400 && (long long)(32 * i) >= (long long)w) break;
        if (S[i]) res += ((seq_t)S[i]) << (idx_t)(32 * i);
    }
    free(S);
    free(T);
    free(U);
    return res;
}
