/*
 * A002832  Median Euler numbers.
 * https://oeis.org/A002832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. 1/(1-c(1)x/(1-c(2)x/(1-c(3)x/...))), c(k) =
 *            ceiling(k/2)*(2k+1); contracted to the J-fraction with b(0) =
 *            c(1), b(h) = c(2h)+c(2h+1), lambda(h) = c(2h-1)*c(2h); Motzkin
 *            table M(0,0) = 1, M(k+1,h) = M(k,h-1) + b(h)*M(k,h) +
 *            lambda(h+1)*M(k,h+1); a(n) = M(n-1,0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   a2832_b, a2832_c, a2832_l, alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/a2832_b.h"
#include "../utils/a2832_c.h"
#include "../utils/a2832_l.h"

static seq_t *a2832_row = 0, *a2832_nxt = 0, *a2832_res = 0;
static idx_t a2832_cap = 0, a2832_rcap = 0, a2832_k = 0;
static int a2832_init = 0;

static void a2832_grow_rows(idx_t need) {
    idx_t newcap = 2 * need + 16;
    seq_t *r = alloc_zero(newcap);
    seq_t *x = alloc_zero(newcap);
    idx_t i;
    for (i = 0; i < a2832_cap; i++) r[i] = a2832_row[i];
    a2832_row = r;
    a2832_nxt = x;
    a2832_cap = newcap;
}

static void a2832_grow_res(idx_t need) {
    idx_t newcap = 2 * need + 16;
    seq_t *r = alloc_zero(newcap);
    idx_t i;
    for (i = 0; i < a2832_rcap; i++) r[i] = a2832_res[i];
    a2832_res = r;
    a2832_rcap = newcap;
}

seq_t a(idx_t n) {
    idx_t m, k, h;
    if (n == 0) return 0;
    m = n - 1;
    if (!a2832_init) {
        a2832_grow_rows(4);
        a2832_grow_res(4);
        a2832_row[0] = 1;
        a2832_res[0] = 1;
        a2832_k = 0;
        a2832_init = 1;
    }
    if (m + 1 > a2832_rcap) a2832_grow_res(m + 1);
    while (a2832_k < m) {
        seq_t *tmp;
        k = a2832_k;
        if (k + 3 > a2832_cap) a2832_grow_rows(k + 3);
        for (h = 0; h <= k + 1; h++) {
            seq_t s = 0;
            if (h >= 1) s += a2832_row[h - 1];
            if (h <= k) s += a2832_b(h) * a2832_row[h];
            if (h + 1 <= k) s += a2832_l(h + 1) * a2832_row[h + 1];
            a2832_nxt[h] = s;
        }
        tmp = a2832_row;
        a2832_row = a2832_nxt;
        a2832_nxt = tmp;
        a2832_k = k + 1;
        a2832_res[a2832_k] = a2832_row[0];
    }
    return a2832_res[m];
}
