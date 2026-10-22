/*
 * A005628  Number of chiral planted trees with n nodes.
 * https://oeis.org/A005628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(n) - p(n), where s(x) = Sum s(n) x^n = 1 + x*(s(x)^3 +
 *            2*s(x^3))/3 (A000625, steric planted trees) p(x) = Sum p(n) x^n
 *            = 1 + x*p(x)*s(x^2) (A005627, achiral planted trees) so s(n) =
 *            ([x^(n-1)] s^3 + 2*s((n-1)/3) [if 3 | n-1]) / 3, p(n) =
 *            Sum_{j=0..floor((n-1)/2)} s(j)*p(n-1-2j), s(0) = p...
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

static seq_t *S = 0, *S2 = 0, *C3 = 0, *P = 0;
static idx_t done = 0, cap = 0;
static bool have_inv = false;
static seq_t inv3;

static seq_t div3(seq_t x)
{
    if (seq_is_fixed_width()) {
        if (!have_inv) {
            unsigned __int128 v = 3;
            int i;
            for (i = 0; i < 8; i++)
                v = v * (2 - 3 * v);
            inv3 = u128_to_seq(v);
            have_inv = true;
        }
        return (seq_t)(x * inv3);
    }
    return x / 3;
}

static void ensure(idx_t n)
{
    idx_t m;
    if (n < done) return;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < n + 1) nc *= 2;
        S = grow_zero(S, cap, nc);
        S2 = grow_zero(S2, cap, nc);
        C3 = grow_zero(C3, cap, nc);
        P = grow_zero(P, cap, nc);
        cap = nc;
    }
    for (m = done; m <= n; m++) {
        if (m == 0) {
            S[0] = 1; P[0] = 1; S2[0] = 1; C3[0] = 1;
            continue;
        }
        idx_t k = m - 1, i, j;
        if (k >= 1) {
            seq_t t = 0;
            for (i = 0; i <= k; i++) t += S[i] * S[k - i];
            S2[k] = t;
            t = 0;
            for (i = 0; i <= k; i++) t += S[i] * S2[k - i];
            C3[k] = t;
        }
        seq_t num = C3[k];
        if (k % 3 == 0) num += 2 * S[k / 3];
        S[m] = div3(num);
        seq_t pp = 0;
        for (j = 0; 2 * j <= k; j++) pp += S[j] * P[k - 2 * j];
        P[m] = pp;
    }
    done = n + 1;
}

seq_t a(idx_t n)
{
    ensure(n);
    return S[n] - P[n];
}
