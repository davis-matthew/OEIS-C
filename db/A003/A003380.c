/*
 * A003380  Numbers that are the sum of 2 nonzero 8th powers.
 * https://oeis.org/A003380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   no closed form; a search that merges all sums i^8 + j^8 (1 <= i
 *            <= j) in increasing order with a min-heap and drops duplicates
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 119, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3380_pow8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a3380_pow8.h"

typedef unsigned __int128 a3380_u128;

static a3380_u128 *a3380_hk = NULL;
static idx_t *a3380_hi = NULL, *a3380_hj = NULL;
static idx_t a3380_hn = 0, a3380_hcap = 0;

static a3380_u128 a3380_p8(idx_t x) {
    a3380_u128 b = (a3380_u128)x, r = 1;
    int k;
    for (k = 0; k < 8; k++) r *= b;
    return r;
}

static void a3380_push(idx_t i, idx_t j) {
    a3380_u128 s = a3380_p8(i) + a3380_p8(j);
    idx_t c, p;
    if (a3380_hn == a3380_hcap) {
        idx_t nc = a3380_hcap ? 2 * a3380_hcap : 64;
        a3380_hk = (a3380_u128 *)realloc(a3380_hk, nc * sizeof(a3380_u128));
        a3380_hi = (idx_t *)realloc(a3380_hi, nc * sizeof(idx_t));
        a3380_hj = (idx_t *)realloc(a3380_hj, nc * sizeof(idx_t));
        a3380_hcap = nc;
    }
    c = a3380_hn++;
    while (c > 0) {
        p = (c - 1) / 2;
        if (!(s < a3380_hk[p])) break;
        a3380_hk[c] = a3380_hk[p];
        a3380_hi[c] = a3380_hi[p];
        a3380_hj[c] = a3380_hj[p];
        c = p;
    }
    a3380_hk[c] = s;
    a3380_hi[c] = i;
    a3380_hj[c] = j;
}

static void a3380_pop(a3380_u128 *k, idx_t *i, idx_t *j) {
    a3380_u128 lk;
    idx_t li, lj, c = 0;
    *k = a3380_hk[0];
    *i = a3380_hi[0];
    *j = a3380_hj[0];
    a3380_hn--;
    if (a3380_hn == 0) return;
    lk = a3380_hk[a3380_hn];
    li = a3380_hi[a3380_hn];
    lj = a3380_hj[a3380_hn];
    for (;;) {
        idx_t l = 2 * c + 1, m;
        if (l >= a3380_hn) break;
        m = l;
        if (l + 1 < a3380_hn && a3380_hk[l + 1] < a3380_hk[l]) m = l + 1;
        if (!(a3380_hk[m] < lk)) break;
        a3380_hk[c] = a3380_hk[m];
        a3380_hi[c] = a3380_hi[m];
        a3380_hj[c] = a3380_hj[m];
        c = m;
    }
    a3380_hk[c] = lk;
    a3380_hi[c] = li;
    a3380_hj[c] = lj;
}

seq_t a(idx_t n) {
    static idx_t *ci = NULL, *cj = NULL;
    static idx_t clen = 0, ccap = 0;
    static a3380_u128 last = 0;
    static bool init = false;

    if (n == 0) return 0;
    if (!init) {
        init = true;
        a3380_push(1, 1);
    }
    while (clen < n) {
        a3380_u128 k;
        idx_t i, j;
        a3380_pop(&k, &i, &j);
        a3380_push(i, j + 1);
        if (j == i) a3380_push(i + 1, i + 1);
        if (clen == 0 || k != last) {
            if (clen == ccap) {
                idx_t nc = ccap ? 2 * ccap : 1024;
                ci = (idx_t *)realloc(ci, nc * sizeof(idx_t));
                cj = (idx_t *)realloc(cj, nc * sizeof(idx_t));
                ccap = nc;
            }
            ci[clen] = i;
            cj[clen] = j;
            clen++;
            last = k;
        }
    }
    return a3380_pow8(ci[n - 1]) + a3380_pow8(cj[n - 1]);
}
