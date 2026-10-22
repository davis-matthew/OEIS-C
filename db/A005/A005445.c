/*
 * A005445  From a Fibonacci-like differential equation.
 * https://oeis.org/A005445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} Stirling1(n,k)*k!*Fibonacci(k), with
 *            Stirling1(i,k) = Stirling1(i-1,k-1) - (i-1)*Stirling1(i-1,k),
 *            Stirling1(0,0) = 1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 32,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *A005445_row = 0;   /* Stirling1(r, k), k = 0..r */
static seq_t *A005445_w = 0;     /* k! * F(k) */
static seq_t *A005445_res = 0;   /* a(0..r) */
static idx_t A005445_cap = 0;
static idx_t A005445_r = 0;      /* current Stirling row index */
static idx_t A005445_wn = 0;     /* number of w entries computed */
static seq_t A005445_fact;       /* (wn-1)! */
static seq_t A005445_fa;         /* F(wn-1) */
static seq_t A005445_fb;         /* F(wn-2) */

static void A005445_ensure(idx_t need) {
    if (need <= A005445_cap) return;
    idx_t nc = A005445_cap * 2;
    if (nc < need) nc = need;
    if (nc < 16) nc = 16;
    if (A005445_cap == 0) {
        A005445_row = alloc_zero(nc);
        A005445_w = alloc_zero(nc);
        A005445_res = alloc_zero(nc);
    } else {
        A005445_row = grow_zero(A005445_row, A005445_cap, nc);
        A005445_w = grow_zero(A005445_w, A005445_cap, nc);
        A005445_res = grow_zero(A005445_res, A005445_cap, nc);
    }
    A005445_cap = nc;
}

seq_t a(idx_t n) {
    if (A005445_cap == 0) {
        A005445_ensure(16);
        A005445_row[0] = 1;
        A005445_res[0] = 0;
        A005445_r = 0;
        A005445_w[0] = 0;          /* 0! * F(0) */
        A005445_wn = 1;
        A005445_fact = 1;          /* 0! */
        A005445_fa = 0;            /* F(0) */
        A005445_fb = 1;            /* F(-1) */
    }
    if (n <= A005445_r) return A005445_res[n];
    A005445_ensure(n + 1);
    while (A005445_wn <= n) {
        idx_t k = A005445_wn;
        seq_t f = A005445_fa + A005445_fb;
        A005445_fb = A005445_fa;
        A005445_fa = f;
        A005445_fact = A005445_fact * (seq_t)k;
        A005445_w[k] = A005445_fact * f;
        A005445_wn++;
    }
    while (A005445_r < n) {
        idx_t r = A005445_r;
        seq_t *row = A005445_row;
        /* row r -> row r+1, in place, high k first */
        for (idx_t k = r + 1; k >= 1; k--) {
            row[k] = row[k - 1] - (seq_t)r * row[k];
        }
        row[0] = 0;
        A005445_r = r + 1;
        seq_t s = 0;
        for (idx_t k = 1; k <= r + 1; k++) {
            s += row[k] * A005445_w[k];
        }
        A005445_res[r + 1] = s;
    }
    return A005445_res[n];
}
