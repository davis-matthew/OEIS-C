/*
 * A007131  Number of unlabeled connected strength 3 Eulerian graphs with n
 *          nodes.
 * https://oeis.org/A007131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = T(n) - Sum_{i=1..n-1} a(i)*E(n-i), a(1) = 0, where E(n)
 *            and T(n) are the numbers of unlabeled multigraphs with edge
 *            multiplicity <= 3 on n nodes having 0, resp. exactly 2, odd
 *            vertices (connected Euler-trail graphs; any other components are
 *            even graphs). By Burnside over cycle types lambda...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   alloc_zero, big_add_shift, big_divsmall, big_mulsmall,
 *            big_to_seq, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/big_add_shift.h"
#include "../utils/big_divsmall.h"
#include "../utils/big_mulsmall.h"
#include "../utils/big_to_seq.h"

static uint32_t *bf, *bt, *bE, *bT;
static size_t fl, al;
static int *pp, *pm;
static int np;

static uint32_t pgcd(uint32_t x, uint32_t y)
{
    while (y) { uint32_t t = x % y; x = y; y = t; }
    return x;
}

static void visit(void)
{
    uint64_t N = 0, e, r;
    int ev = 0, od = 0, m1 = 0, m2 = 0, i, j, t;
    size_t xlen;
    uint32_t f;
    for (i = 0; i < np; i++) {
        uint64_t k = (uint64_t)pp[i], m = (uint64_t)pm[i];
        N += m * (k / 2) + k * (m * (m - 1) / 2);
        for (j = 0; j < i; j++)
            N += m * (uint64_t)pm[j] * pgcd((uint32_t)k, (uint32_t)pp[j]);
        if (k % 2 == 0) ev += (int)m; else od += (int)m;
        if (k == 1) m1 = (int)m;
        if (k == 2) m2 = (int)m;
    }
    r = (uint64_t)ev + (uint64_t)(od > 0 ? od - 1 : 0);
    e = 2 * N - r;
    memcpy(bt, bf, fl * sizeof(uint32_t));
    bt[fl] = 0;
    bt[fl + 1] = 0;
    for (i = 0; i < np; i++) {
        for (t = 0; t < pm[i]; t++) big_divsmall(bt, fl, (uint32_t)pp[i]);
        for (t = 2; t <= pm[i]; t++) big_divsmall(bt, fl, (uint32_t)t);
    }
    xlen = fl;
    while (xlen > 1 && bt[xlen - 1] == 0) xlen--;
    big_add_shift(bE, al, bt, xlen, e);
    f = (uint32_t)m2 + (uint32_t)(m1 * (m1 - 1) / 2);
    if (f) {
        bt[xlen] = big_mulsmall(bt, xlen, f);
        xlen++;
        big_add_shift(bT, al, bt, xlen, e);
    }
}

static void gen(int rem, int maxp)
{
    int k, m;
    if (rem == 0) { visit(); return; }
    for (k = maxp < rem ? maxp : rem; k >= 1; k--) {
        for (m = 1; m * k <= rem; m++) {
            pp[np] = k; pm[np] = m; np++;
            gen(rem - m * k, k - 1);
            np--;
        }
    }
}

static void compute_level(int k, seq_t *Eout, seq_t *Tout)
{
    size_t fb = 0;
    int i;
    for (i = 2; i <= k; i++) {
        int b = 0, x = i;
        while (x) { b++; x >>= 1; }
        fb += (size_t)b;
    }
    fl = fb / 32 + 2;
    al = (fb + (size_t)k * (size_t)(k - 1) + 64) / 32 + 4;
    bf = (uint32_t *)calloc(fl, sizeof(uint32_t));
    bt = (uint32_t *)calloc(fl + 2, sizeof(uint32_t));
    bE = (uint32_t *)calloc(al, sizeof(uint32_t));
    bT = (uint32_t *)calloc(al, sizeof(uint32_t));
    pp = (int *)malloc(sizeof(int) * (size_t)(k + 1));
    pm = (int *)malloc(sizeof(int) * (size_t)(k + 1));
    bf[0] = 1;
    for (i = 2; i <= k; i++) big_mulsmall(bf, fl, (uint32_t)i);
    np = 0;
    gen(k, k);
    for (i = 2; i <= k; i++) {
        big_divsmall(bE, al, (uint32_t)i);
        big_divsmall(bT, al, (uint32_t)i);
    }
    *Eout = big_to_seq(bE, al);
    *Tout = big_to_seq(bT, al);
    free(bf); free(bt); free(bE); free(bT); free(pp); free(pm);
}

static seq_t *EE = NULL, *TT = NULL, *CC = NULL;
static idx_t cap = 0, done = 0;

seq_t a(idx_t n)
{
    idx_t k, i;
    if (n == 0) return 0;
    if (n > done) {
        if (n + 1 > cap) {
            if (cap == 0) {
                EE = alloc_zero(n + 1);
                TT = alloc_zero(n + 1);
                CC = alloc_zero(n + 1);
                EE[0] = 1;
            } else {
                EE = grow_zero(EE, cap, n + 1);
                TT = grow_zero(TT, cap, n + 1);
                CC = grow_zero(CC, cap, n + 1);
            }
            cap = n + 1;
        }
        for (k = done + 1; k <= n; k++) {
            seq_t e, t;
            compute_level((int)k, &e, &t);
            EE[k] = e;
            TT[k] = t;
            CC[k] = TT[k];
            for (i = 1; i < k; i++) CC[k] -= CC[i] * EE[k - i];
        }
        done = n;
    }
    return CC[n];
}
