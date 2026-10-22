/*
 * A002986  Number of non-cyclic hydrocarbons with n carbon atoms (excluding
 *          stereoisomers).
 * https://oeis.org/A002986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = R4(n) - Sum_{b=1..3} E_b(n), where the children of a
 *            carbon atom are counted as a multiset, each child using bond
 *            order b (1..3) at a total valence <= 4. C_b(s) = Sum_{w=0..4-b}
 *            f(s-1,w) counts the planted subtrees that hang off a parent by a
 *            bond of order b; f(m,w) counts multisets of chi...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 65,
 *            big any n
 * Helpers:   A_grow, mc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/A_grow.h"
#include "../utils/mc.h"

static seq_t *F = NULL, *Cv = NULL;
static idx_t Flen = 0, Clen = 0, built = 0;

static seq_t binom2(seq_t c)
{
    if (c % 2 == 0) return (c / 2) * (c - 1);
    return c * ((c - 1) / 2);
}

static void build(idx_t N)
{
    idx_t fl = N * 5, cl = N * 3, i, s, m;
    int b, w, j, jmax;
    if (fl > Flen) { F = A_grow(F, Flen, fl); Flen = fl; }
    if (cl > Clen) { Cv = A_grow(Cv, Clen, cl); Clen = cl; }
    for (i = 0; i < fl; i++) F[i] = 0;
    for (i = 0; i < cl; i++) Cv[i] = 0;
    F[0] = 1;
    for (s = 1; s < N; s++) {
        for (b = 1; b <= 3; b++) {
            seq_t c = 0;
            for (w = 0; w <= 4 - b; w++) c += F[(s - 1) * 5 + w];
            Cv[(idx_t)(b - 1) * N + s] = c;
        }
        for (b = 1; b <= 3; b++) {
            seq_t c = Cv[(idx_t)(b - 1) * N + s];
            seq_t mm[5];
            jmax = 4 / b;
            mm[0] = 1;
            mm[1] = c;
            for (j = 2; j <= jmax; j++) {
                if ((idx_t)j * s < N) mm[j] = mc(c, j);
                else mm[j] = 0;
            }
            for (m = N - 1; m >= s; m--) {
                for (w = 4; w >= b; w--) {
                    seq_t add = 0;
                    for (j = 1; j <= jmax && (idx_t)j * s <= m && j * b <= w; j++)
                        add += mm[j] * F[(m - (idx_t)j * s) * 5 + (idx_t)(w - j * b)];
                    F[m * 5 + (idx_t)w] += add;
                }
            }
        }
    }
    built = N;
}

seq_t a(idx_t n)
{
    seq_t R4 = 0, E = 0;
    int b, w;
    idx_t s;
    if (n < 1) return 0;
    if (n > built) {
        idx_t N = n;
        if (N < 2 * built) N = 2 * built;
        if (N < 64) N = 64;
        build(N);
    }
    for (w = 0; w <= 4; w++) R4 += F[(n - 1) * 5 + (idx_t)w];
    for (b = 1; b <= 3; b++) {
        const seq_t *C = Cv + (idx_t)(b - 1) * built;
        for (s = 1; 2 * s < n; s++) E += C[s] * C[n - s];
        if (n % 2 == 0) E += binom2(C[n / 2]);
    }
    return R4 - E;
}
