/*
 * A007121  Expansion of e.g.f. ( (1+x)^x )^x.
 * https://oeis.org/A007121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Sum_{k=3..n} binomial(n-1,k-1) * (-1)^(k-1) *
 *            k*(k-1)*(k-3)! * a(n-k) (from A' = (x^2*log(1+x))' * A, with A
 *            the e.g.f.; this is the OEIS recurrence with k!/(k-2) =
 *            k*(k-1)*(k-3)! kept integral). In fixed-width builds the
 *            arithmetic is done mod 2^128 in unsigned __int128, so larg...
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 15, i64 n <= 23, i128 n <= 36,
 *            big any n
 * Helpers:   from_i128, grow_zero, seq_is_big, zalloc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/from_i128.h"
#include "../utils/zalloc_s.h"
#include "../utils/grow_zero.h"

typedef unsigned __int128 u128_t;

/* ---- fixed-width path: exact modulo 2^128 ---- */
static u128_t *ua = NULL, *urow = NULL, *ug = NULL;
static u128_t ufact = 1;
static idx_t ucnt = 0, ucap = 0;

static void u_fill(idx_t n) {
    if (n + 1 > ucap) {
        idx_t nc = (n + 1) * 2;
        ua = (u128_t *)realloc(ua, nc * sizeof(u128_t));
        urow = (u128_t *)realloc(urow, nc * sizeof(u128_t));
        ug = (u128_t *)realloc(ug, nc * sizeof(u128_t));
        for (idx_t i = ucap; i < nc; i++) { ua[i] = 0; urow[i] = 0; ug[i] = 0; }
        ucap = nc;
    }
    while (ucnt <= n) {
        idx_t m = ucnt;
        if (m >= 3) {
            if (m == 3) ufact = 1; else ufact *= (u128_t)(m - 3);
            u128_t g = (u128_t)m * (u128_t)(m - 1) * ufact;
            ug[m] = (m % 2 == 0) ? (u128_t)0 - g : g;
        }
        if (m == 0) {
            ua[0] = 1;
        } else {
            /* make urow hold row m-1 of Pascal's triangle */
            if (m == 1) {
                urow[0] = 1;
            } else {
                urow[m - 1] = 0;
                for (idx_t j = m - 1; j >= 1; j--) urow[j] += urow[j - 1];
            }
            u128_t s = 0;
            for (idx_t k = 3; k <= m; k++)
                s += urow[k - 1] * ug[k] * ua[m - k];
            ua[m] = s;
        }
        ucnt++;
    }
}

/* ---- arbitrary-precision path: exact ---- */
static seq_t *ba = NULL, *brow = NULL, *bg = NULL;
static idx_t bcnt = 0, bcap = 0;

static void b_fill(idx_t n) {
    static seq_t bfact;
    if (n + 1 > bcap) {
        idx_t nc = (n + 1) * 2;
        if (ba == NULL) {
            ba = zalloc_s(nc); brow = zalloc_s(nc); bg = zalloc_s(nc);
        } else {
            ba = grow_zero(ba, bcap, nc);
            brow = grow_zero(brow, bcap, nc);
            bg = grow_zero(bg, bcap, nc);
        }
        bcap = nc;
    }
    while (bcnt <= n) {
        idx_t m = bcnt;
        if (m >= 3) {
            if (m == 3) bfact = 1; else bfact *= (seq_t)(m - 3);
            seq_t g = (seq_t)m * (seq_t)(m - 1) * bfact;
            if (m % 2 == 0) g = (seq_t)0 - g;
            bg[m] = g;
        }
        if (m == 0) {
            ba[0] = 1;
        } else {
            if (m == 1) {
                brow[0] = 1;
            } else {
                brow[m - 1] = 0;
                for (idx_t j = m - 1; j >= 1; j--) brow[j] += brow[j - 1];
            }
            seq_t s = 0;
            for (idx_t k = 3; k <= m; k++) {
                seq_t t = brow[k - 1] * bg[k];
                s += t * ba[m - k];
            }
            ba[m] = s;
        }
        bcnt++;
    }
}

seq_t a(idx_t n) {
    if (seq_is_big()) {
        b_fill(n);
        return ba[n];
    }
    u_fill(n);
    u128_t r = ua[n];
    unsigned w = (unsigned)(8 * sizeof(seq_t));
    __int128 s;
    if (w < 128) {
        u128_t mask = (((u128_t)1) << w) - 1;
        u128_t mm = r & mask;
        if ((mm >> (w - 1)) & 1)
            s = (__int128)mm - (((__int128)1) << w);
        else
            s = (__int128)mm;
    } else {
        s = (__int128)r;
    }
    return from_i128(s);
}
