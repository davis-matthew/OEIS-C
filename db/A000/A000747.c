/*
 * A000747  Boustrophedon transform of primes.
 * https://oeis.org/A000747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon transform of b(k) = prime(k+1): T(k,0) =
 *            prime(k+1), T(k,j) = T(k,j-1) + T(k-1,k-j) for 1 <= j <= k, a(n)
 *            = T(n,n); equivalently a(n) = Sum_{k=0..n}
 *            A109449(n,k)*A000040(k+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   bt_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/bt_grow.h"

static uint64_t *bt_pr = NULL;
static idx_t bt_npr = 0;

static void bt_ensure_primes(idx_t cnt) {
    uint64_t lim = 1024;
    while (bt_npr < cnt) {
        lim *= 2;
        char *comp = (char *)calloc((size_t)lim + 1, 1);
        idx_t c = 0;
        for (uint64_t i = 2; i <= lim; i++) {
            if (!comp[i]) {
                c++;
                for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
            }
        }
        if (c >= cnt) {
            free(bt_pr);
            bt_pr = (uint64_t *)malloc((size_t)c * sizeof(uint64_t));
            idx_t k = 0;
            for (uint64_t i = 2; i <= lim; i++)
                if (!comp[i]) bt_pr[k++] = i;
            bt_npr = c;
        }
        free(comp);
    }
}

static seq_t *bt_prev = NULL, *bt_cur = NULL, *bt_vals = NULL;
static idx_t bt_done = 0, bt_cap = 0;

seq_t a(idx_t n) {
    if (n < bt_done) return bt_vals[n];
    if (n + 1 > bt_cap) {
        idx_t nc = bt_cap * 2;
        if (nc < n + 1) nc = n + 1;
        bt_prev = bt_grow(bt_prev, bt_done, nc);
        bt_cur = bt_grow(bt_cur, bt_done, nc);
        bt_vals = bt_grow(bt_vals, bt_done, nc);
        bt_cap = nc;
    }
    bt_ensure_primes(n + 1);
    for (idx_t k = bt_done; k <= n; k++) {
        bt_cur[0] = (seq_t)bt_pr[k];
        for (idx_t j = 1; j <= k; j++)
            bt_cur[j] = bt_cur[j - 1] + bt_prev[k - j];
        bt_vals[k] = bt_cur[k];
        seq_t *t = bt_prev;
        bt_prev = bt_cur;
        bt_cur = t;
    }
    bt_done = n + 1;
    return bt_vals[n];
}
