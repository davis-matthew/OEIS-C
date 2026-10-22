/*
 * A007241  McKay-Thompson series of class 2A for the Monster group with a(0)
 *          = 24.
 * https://oeis.org/A007241
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = r(n+1) + 4096*p(n-1) for n >= 1, a(-1) = 1, a(0) = 24,
 *            where Sum p(k) q^k = Product_{k>=1} (1+q^k)^24 and Sum r(k) q^k
 *            = Product_{k>=1} (1+q^k)^(-24) = Product_{k>=1} (1-q^(2k-1))^24,
 *            i.e. G.f. = T + 4096/T + 48 with T = 1/q * Product_{k>=1}
 *            (1+q^k)^(-24). Since r(k) = (-1)^k s(k), s(k) =...
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 28, u128 n <= 108,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

/* ---------- arbitrary precision path: incremental log-derivative ---------- */
static seq_t *Pb = 0, *Sb = 0;
static idx_t lenB = 0;

static void extend_big(idx_t need)
{
    idx_t nl, d, e, m, j, start;
    long long *c1, *c2;
    if (need <= lenB) return;
    nl = need < lenB + 32 ? lenB + 32 : need;
    c1 = (long long *)calloc((size_t)nl, sizeof(long long));
    c2 = (long long *)calloc((size_t)nl, sizeof(long long));
    for (d = 1; d < nl; d++)
        for (e = 1; d * e < nl; e++) {
            long long v = (e & 1) ? (long long)d : -(long long)d;
            c1[d * e] += v;
            if (d & 1) c2[d * e] += v;
        }
    if (lenB == 0) {
        Pb = alloc_zero(nl);
        Sb = alloc_zero(nl);
    } else {
        Pb = grow_zero(Pb, lenB, nl);
        Sb = grow_zero(Sb, lenB, nl);
    }
    start = lenB;
    if (start == 0) {
        Pb[0] = 1;
        Sb[0] = 1;
        start = 1;
    }
    for (m = start; m < nl; m++) {
        seq_t pp = 0, pn = 0, sp = 0, sn = 0;
        for (j = 1; j <= m; j++) {
            long long a1 = c1[j], a2 = c2[j];
            if (a1 > 0) pp += Pb[m - j] * (idx_t)(24 * a1);
            else if (a1 < 0) pn += Pb[m - j] * (idx_t)(-24 * a1);
            if (a2 > 0) sp += Sb[m - j] * (idx_t)(24 * a2);
            else if (a2 < 0) sn += Sb[m - j] * (idx_t)(-24 * a2);
        }
        Pb[m] = (pp - pn) / m;
        Sb[m] = (sp - sn) / m;
    }
    free(c1);
    free(c2);
    lenB = nl;
}

/* ---------- fixed width path: division-free, exact mod 2^128 ---------- */
/* For n > 1000 the term exceeds 2^128 by far, so no fixed width can hold it. */
#define LIMF 1002
static unsigned __int128 *Pf = 0, *Sf = 0;

static void build_fixed(void)
{
    unsigned __int128 C[25];
    idx_t k, m, j;
    if (Pf) return;
    C[0] = 1;
    for (j = 1; j <= 24; j++) C[j] = C[j - 1] * (unsigned)(25 - j) / (unsigned)j;
    Pf = (unsigned __int128 *)calloc(LIMF, sizeof(unsigned __int128));
    Sf = (unsigned __int128 *)calloc(LIMF, sizeof(unsigned __int128));
    Pf[0] = 1;
    Sf[0] = 1;
    for (k = 1; k < LIMF; k++) {
        for (m = LIMF - 1; m >= k; m--) {
            unsigned __int128 t = Pf[m], u = Sf[m];
            for (j = 1; j <= 24 && j * k <= m; j++) {
                t += C[j] * Pf[m - j * k];
                if (k & 1) u += C[j] * Sf[m - j * k];
            }
            Pf[m] = t;
            Sf[m] = u;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == -1) return 1;
    if (n == 0) return 24;
    if (seq_is_fixed_width()) {
        unsigned __int128 v;
        if (n + 1 >= LIMF) return 0;
        build_fixed();
        v = Pf[n - 1] * 4096u;
        if (n & 1) v += Sf[n + 1];
        else v -= Sf[n + 1];
        return u128_to_seq(v);
    } else {
        seq_t t;
        extend_big(n + 2);
        t = Pb[n - 1] * 4096;
        if (n & 1) return t + Sb[n + 1];
        return t - Sb[n + 1];
    }
}
