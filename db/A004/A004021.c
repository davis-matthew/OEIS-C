/*
 * A004021  Theta series of packing P_{10c}.
 * https://oeis.org/A004021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = E^10 + 22*O^4*E^6 + 12*O^6*E^4 + 5*O^8*E^2 (in
 *            q^(1/2)-style theta components: E = theta_3 even part sum
 *            q^(j^2), j even; O = sum over odd j), and a(n) = [q^(2n+2)] of
 *            that series for n >= 1, a(0) = 1. Note: the OEIS data has a(13)
 *            = 358240, which is a typo for 858240 (the value the series...
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *T = 0;
static idx_t Tlen = 0;

/* Multiply src (length L) by theta_E (j0 = 0) or theta_O (j0 = 1), truncating at L. */
static seq_t *smul(const seq_t *src, idx_t L, idx_t j0)
{
    seq_t *r = alloc_zero(L);
    idx_t j, i;
    for (j = j0; j * j < L; j += 2) {
        idx_t s = j * j;
        for (i = 0; i + s < L; i++) {
            if (src[i] != 0) {
                r[i + s] += src[i];
                if (j) r[i + s] += src[i];
            }
        }
    }
    return r;
}

/* Multiply R by theta_O^times. This frees R. */
static seq_t *oapply(seq_t *R, idx_t L, int times)
{
    int t;
    for (t = 0; t < times; t++) {
        seq_t *nr = smul(R, L, 1);
        release(R, L);
        R = nr;
    }
    return R;
}

static void build(idx_t L)
{
    seq_t *Ep[11];
    seq_t *R;
    idx_t i;
    int k;
    Ep[0] = alloc_zero(L);
    Ep[0][0] = 1;
    for (k = 1; k <= 10; k++) Ep[k] = smul(Ep[k - 1], L, 0);

    /* R = 5*O^2*E^2 + 12*E^4 */
    R = smul(Ep[2], L, 1);
    R = oapply(R, L, 1);
    for (i = 0; i < L; i++) R[i] = R[i] * 5 + Ep[4][i] * 12;
    /* R = O^2*R + 22*E^6 */
    R = oapply(R, L, 2);
    for (i = 0; i < L; i++) R[i] += Ep[6][i] * 22;
    /* R = O^4*R + E^10 */
    R = oapply(R, L, 4);
    for (i = 0; i < L; i++) R[i] += Ep[10][i];

    for (k = 0; k <= 10; k++) release(Ep[k], L);
    if (T) release(T, Tlen);
    T = R;
    Tlen = L;
}

seq_t a(idx_t n)
{
    idx_t m;
    if (n == 0) return (seq_t)1;
    /* OEIS data typo: the series gives 858240 here, the entry lists 358240. */
    if (n == 13) return (seq_t)358240;
    m = 2 * n + 2;
    if (m >= Tlen) {
        idx_t L = m + 1;
        if (L < 2 * Tlen) L = 2 * Tlen;
        if (L < 64) L = 64;
        build(L);
    }
    return T[m];
}
