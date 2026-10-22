/*
 * A003066  Problimes (first definition).
 * https://oeis.org/A003066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n+1) = a(n) + floor( Product_{i=1..n} a(i)/(a(i)-1) ), a(1) =
 *            2 (exact big-rational arithmetic on Q = Product a(i), P =
 *            Product (a(i)-1))
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   add_n, bitlen32, cmp_n, get64, mul_small, sub_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/add_n.h"
#include "../utils/bitlen32.h"
#include "../utils/cmp_n.h"
#include "../utils/get64.h"
#include "../utils/mul_small.h"
#include "../utils/sub_n.h"

static uint32_t *QQ = 0, *PP = 0, *TT = 0, *T2 = 0;
static size_t QL = 0, PL = 0, CAP = 0;
static uint64_t *TERMS = 0;
static size_t TN = 0, TC = 0;

static void ensure_cap(size_t need) {
    if (need <= CAP) return;
    size_t nc = CAP ? CAP : 16;
    while (nc < need) nc *= 2;
    QQ = (uint32_t *)realloc(QQ, nc * sizeof(uint32_t));
    PP = (uint32_t *)realloc(PP, nc * sizeof(uint32_t));
    TT = (uint32_t *)realloc(TT, nc * sizeof(uint32_t));
    T2 = (uint32_t *)realloc(T2, nc * sizeof(uint32_t));
    CAP = nc;
}

/* floor(QQ / PP), quotient known to be small */
static uint64_t small_quot(void) {
    uint64_t bl = 32 * (uint64_t)(PL - 1) + bitlen32(PP[PL - 1]);
    uint64_t pos = bl > 64 ? bl - 64 : 0;
    unsigned __int128 X = (unsigned __int128)get64(QQ, QL, pos)
                        | ((unsigned __int128)get64(QQ, QL, pos + 64) << 64);
    uint64_t Y = get64(PP, PL, pos);
    uint64_t q = (uint64_t)(X / Y);
    size_t tl, t2l;
    if (q > 0) q--;
    /* TT = q * P */
    memcpy(TT, PP, PL * sizeof(uint32_t));
    tl = PL;
    if (q == 0) tl = 0; else mul_small(TT, &tl, (uint32_t)q);
    while (cmp_n(TT, tl, QQ, QL) > 0) { sub_n(TT, &tl, PP, PL); q--; }
    for (;;) {
        memcpy(T2, TT, tl * sizeof(uint32_t));
        t2l = tl;
        add_n(T2, &t2l, PP, PL);
        if (cmp_n(T2, t2l, QQ, QL) <= 0) {
            memcpy(TT, T2, t2l * sizeof(uint32_t));
            tl = t2l;
            q++;
        } else break;
    }
    return q;
}

static void extend_to(size_t n) {
    if (TN == 0) {
        TC = 64;
        TERMS = (uint64_t *)malloc(TC * sizeof(uint64_t));
        TERMS[0] = 2;
        TN = 1;
        ensure_cap(16);
        QQ[0] = 2; QL = 1;
        PP[0] = 1; PL = 1;
    }
    while (TN < n) {
        uint64_t nx;
        ensure_cap(QL + 8);
        nx = TERMS[TN - 1] + small_quot();
        if (TN >= TC) {
            TC *= 2;
            TERMS = (uint64_t *)realloc(TERMS, TC * sizeof(uint64_t));
        }
        TERMS[TN++] = nx;
        mul_small(QQ, &QL, (uint32_t)nx);
        mul_small(PP, &PL, (uint32_t)(nx - 1));
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    extend_to((size_t)n);
    return (seq_t)TERMS[n - 1];
}
