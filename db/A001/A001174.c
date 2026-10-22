/*
 * A001174  Number of oriented graphs (i.e., digraphs with no bidirected
 *          edges) on n unlabeled nodes. Also number of complete digraphs on n
 *          unlabeled nodes. Number of antisymmetric relations (i.e., oriented
 *          graphs with loops) on n unlabeled nodes is A083670.
 * https://oeis.org/A001174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   o1174_add, o1174_divs, o1174_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (Harary & Palmer, Eq. 5.4.14): a(n) = Sum_{partitions j of n} 3^E(j) / z(j),
   with z(j) = Prod_k k^j_k * j_k!, and
   E(j) = Sum_k j_k*floor((k-1)/2) + Sum_k k*C(j_k,2) + Sum_{r<s} j_r*j_s*gcd(r,s).
   Computed as (Sum_j (n!/z(j)) * 3^E(j)) / n! with exact multi-limb arithmetic. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/o1174_add.h"
#include "../utils/o1174_divs.h"
#include "../utils/o1174_gcd.h"

static int o1174_L;
static uint32_t *o1174_C;
static uint32_t *o1174_pool;
static int *o1174_len;
static int *o1174_mul;
static int o1174_nd;

static void o1174_rec(int rem, int maxk, int depth, uint64_t e) {
    int L = o1174_L;
    const uint32_t *cur = o1174_pool + (size_t)depth * L;
    if (rem == 0) {
        o1174_add(o1174_C + (size_t)e * L, cur, L);
        return;
    }
    uint32_t *nx = o1174_pool + (size_t)(depth + 1) * L;
    int top = rem < maxk ? rem : maxk;
    for (int k = top; k >= 1; k--) {
        uint64_t base = (uint64_t)(k - 1) / 2;
        for (int i = 0; i < o1174_nd; i++)
            base += (uint64_t)o1174_mul[i] * o1174_gcd((uint64_t)k, (uint64_t)o1174_len[i]);
        memcpy(nx, cur, (size_t)L * sizeof(uint32_t));
        uint64_t ee = e;
        o1174_len[o1174_nd] = k;
        o1174_nd++;
        for (int m = 1; m * k <= rem; m++) {
            ee += base + (uint64_t)k * (uint64_t)(m - 1);
            o1174_divs(nx, L, (uint32_t)(k * m));
            o1174_mul[o1174_nd - 1] = m;
            o1174_rec(rem - m * k, k - 1, depth + 1, ee);
        }
        o1174_nd--;
    }
}

seq_t a(idx_t n) {
    int nn = (int)n;
    int bl = 1;
    while ((1 << bl) <= nn) bl++;
    int L = (nn * bl) / 32 + 2;
    uint64_t Emax = (uint64_t)nn * (uint64_t)(nn - 1) / 2;

    o1174_L = L;
    o1174_C = (uint32_t *)calloc((size_t)(Emax + 1) * L, sizeof(uint32_t));
    o1174_pool = (uint32_t *)calloc((size_t)(nn + 2) * L, sizeof(uint32_t));
    o1174_len = (int *)calloc((size_t)nn + 2, sizeof(int));
    o1174_mul = (int *)calloc((size_t)nn + 2, sizeof(int));
    o1174_nd = 0;

    /* pool[0] = n! */
    o1174_pool[0] = 1;
    for (int i = 2; i <= nn; i++) {
        uint64_t c = 0;
        for (int j = 0; j < L; j++) {
            c += (uint64_t)o1174_pool[j] * (uint64_t)i;
            o1174_pool[j] = (uint32_t)c;
            c >>= 32;
        }
    }

    o1174_rec(nn, nn, 0, 0);

    /* T = Sum_e C[e] * 3^e by Horner */
    int LT = L + (int)((Emax * 2) / 32) + 2;
    uint32_t *T = (uint32_t *)calloc((size_t)LT, sizeof(uint32_t));
    int used = 1;
    for (uint64_t ei = Emax + 1; ei-- > 0;) {
        uint64_t c = 0;
        for (int j = 0; j < used; j++) {
            c += (uint64_t)T[j] * 3u;
            T[j] = (uint32_t)c;
            c >>= 32;
        }
        if (c && used < LT) { T[used] = (uint32_t)c; used++; }
        const uint32_t *ce = o1174_C + (size_t)ei * L;
        c = 0;
        int j = 0;
        for (; j < L && j < LT; j++) {
            c += (uint64_t)T[j] + ce[j];
            T[j] = (uint32_t)c;
            c >>= 32;
        }
        for (; c && j < LT; j++) {
            c += (uint64_t)T[j];
            T[j] = (uint32_t)c;
            c >>= 32;
        }
        if (j > used) used = j;
        while (used < LT && T[used]) used++;
        while (used > 1 && T[used - 1] == 0) used--;
    }

    for (int i = 2; i <= nn; i++) o1174_divs(T, LT, (uint32_t)i);

    int topi = LT - 1;
    while (topi > 0 && T[topi] == 0) topi--;
    seq_t r = 0;
    for (int i = topi; i >= 0; i--) {
        r = r * (seq_t)65536 + (seq_t)(T[i] >> 16);
        r = r * (seq_t)65536 + (seq_t)(T[i] & 0xffffu);
    }

    free(T);
    free(o1174_C);
    free(o1174_pool);
    free(o1174_len);
    free(o1174_mul);
    return r;
}
