/*
 * A000296  Set partitions without singletons: number of partitions of an
 *          n-set into blocks of size > 1. Also number of cyclically spaced
 *          (or feasible) partitions.
 * https://oeis.org/A000296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Bell(n-1) - a(n-1), a(0) = 1, where B(n) = a(n) + a(n+1);
 *            Bell numbers via the Bell (Aitken) triangle
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   zalloc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/zalloc_s.h"

static seq_t *bt_row = 0;   /* current row R of the Bell triangle; row[0] = Bell(R) */
static seq_t *a_vals = 0;   /* a(0..na-1) */
static idx_t bt_cap = 0;
static idx_t bt_R = 0;
static idx_t a_na = 0;

static void a296_reserve(idx_t need)
{
    idx_t nc, i;
    seq_t *nr, *na;
    if (need <= bt_cap) return;
    nc = bt_cap ? bt_cap : 16;
    while (nc < need) nc *= 2;
    nr = zalloc_s(nc);
    na = zalloc_s(nc);
    for (i = 0; i < bt_cap; i++) {
        nr[i] = bt_row[i];
        na[i] = a_vals[i];
    }
    bt_row = nr;
    a_vals = na;
    bt_cap = nc;
}

static void a296_advance(void)
{
    /* new[0] = old[R]; new[c] = new[c-1] + old[c-1] */
    idx_t R = bt_R, c;
    seq_t prev = bt_row[0];
    seq_t cur;
    bt_row[0] = bt_row[R];
    for (c = 1; c <= R + 1; c++) {
        if (c <= R) cur = bt_row[c];
        bt_row[c] = bt_row[c - 1] + prev;
        if (c <= R) prev = cur;
    }
    bt_R = R + 1;
}

seq_t a(idx_t n)
{
    a296_reserve(n + 3);
    if (a_na == 0) {
        bt_row[0] = 1;
        bt_R = 0;
        a_vals[0] = 1;
        a_na = 1;
    }
    while (a_na <= n) {
        while (bt_R < a_na - 1) a296_advance();
        a_vals[a_na] = bt_row[0] - a_vals[a_na - 1];
        a_na++;
    }
    return a_vals[n];
}
