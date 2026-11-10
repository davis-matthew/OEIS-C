/*
 * A205596  Least k such that A205592(k) = 2^n.
 * https://oeis.org/A205596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2; for n >= 1, a(n) = (3*m + 1)/2 where m = least odd k
 *            >= 3 with A205592(k) = 2^(n-1). (A205592: a(2)=1,
 *            a(3k)=a(3k+1)=a(2k), a(3k+2)=2a(2k+1).) Computed by a bounded
 *            search through the inverse map: even j -> 3j/2, 3j/2+1 (same
 *            exponent); odd j >= 3 -> (3j+1)/2 (exponent + 1)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A205596_oddv = NULL;   /* least odd index >= 3 with value 2^e */
static size_t A205596_oddn = 0;
static size_t A205596_oddcap = 0;

static void A205596_vpush(seq_t **v, size_t *n, size_t *cap, seq_t x)
{
    if (*n == *cap) {
        size_t nc = *cap ? *cap * 2 : 64;
        seq_t *t = mem_resize(seq_t, *v, nc);
        if (!t) exit(1);
        *v = t;
        *cap = nc;
    }
    (*v)[(*n)++] = x;
}

/* Least odd index >= 3 with A205592 value 2^e, searching only below bound B;
   returns 0 if none found within the bound. */
static seq_t A205596_find_odd(size_t e, seq_t B)
{
    seq_t *U = mem_new(seq_t, e + 1);
    U[e] = B;
    for (size_t l = e; l > 0; l--) {
        if (U[l] >= 1) U[l - 1] = (2 * U[l] - 1) / 3;
        else U[l - 1] = 0;
    }

    seq_t *odds = NULL, *nodds = NULL, *st = NULL;
    size_t on = 0, oc = 0, nn = 0, nc = 0, sn = 0, sc = 0;
    seq_t best = 0;

    for (size_t l = 0; l <= e; l++) {
        nn = 0;
        sn = 0;
        if (l == 0) {
            if (U[0] >= 2) A205596_vpush(&st, &sn, &sc, (seq_t)2);
        } else {
            for (size_t i = 0; i < on; i++)
                A205596_vpush(&st, &sn, &sc, (3 * odds[i] + 1) / 2);
        }
        while (sn > 0) {
            seq_t x = st[--sn];
            if (x % 2 == 1) {
                if (l == e) {
                    if (best == 0 || x < best) best = x;
                } else {
                    A205596_vpush(&nodds, &nn, &nc, x);
                }
            } else {
                seq_t c = 3 * (x / 2);
                if (c <= U[l] && (l != e || best == 0 || c < best))
                    A205596_vpush(&st, &sn, &sc, c);
                if (c + 1 <= U[l] && (l != e || best == 0 || c + 1 < best))
                    A205596_vpush(&st, &sn, &sc, c + 1);
            }
        }
        /* swap odds <-> nodds */
        seq_t *t = odds; odds = nodds; nodds = t;
        size_t tc = oc; oc = nc; nc = tc;
        on = nn;
    }

    mem_free(U);
    if (odds) mem_free(odds);
    if (nodds) mem_free(nodds);
    if (st) mem_free(st);
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    size_t need = (size_t)n;
    while (A205596_oddn < need) {
        size_t e = A205596_oddn;
        seq_t B = (e == 0) ? (seq_t)8 : (3 * A205596_oddv[e - 1] + 8);
        seq_t r;
        for (;;) {
            r = A205596_find_odd(e, B);
            if (r != 0) break;
            B = B + B / 2 + 1;
        }
        if (A205596_oddn == A205596_oddcap) {
            size_t nc = A205596_oddcap ? A205596_oddcap * 2 : 16;
            seq_t *t = mem_resize(seq_t, A205596_oddv, nc);
            if (!t) exit(1);
            A205596_oddv = t;
            A205596_oddcap = nc;
        }
        A205596_oddv[A205596_oddn++] = r;
    }
    return (3 * A205596_oddv[need - 1] + 1) / 2;
}
