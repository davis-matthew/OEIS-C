/*
 * A003722  Expansion of e.g.f. sin(sinh(x)) (odd powers only).
 * https://oeis.org/A003722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * T(2n+1, 2k+1), where T are the
 *            central factorial numbers of the second kind (e.g.f. of column k
 *            is sinh(x)^k/k!), computed by the recurrence T(2m+1, 2j+1) =
 *            T(2m-1, 2j-1) + (2j+1)^2 * T(2m-1, 2j+1), T(1,1) = 1, and T = 0
 *            out of range
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 23,
 *            big any n
 * Helpers:   alloc_zero, from_i128, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"

static unsigned __int128 *f_row = NULL, *f_res = NULL;
static idx_t f_cnt = 0, f_cap = 0;

static seq_t *b_row = NULL, *b_res = NULL;
static idx_t b_cnt = 0, b_cap = 0;

static void fixed_extend(idx_t n) {
    if (n + 1 > f_cap) {
        idx_t nc = f_cap * 2;
        if (nc < n + 1) nc = n + 1;
        f_row = (unsigned __int128 *)realloc(f_row, (size_t)nc * sizeof(unsigned __int128));
        f_res = (unsigned __int128 *)realloc(f_res, (size_t)nc * sizeof(unsigned __int128));
        f_cap = nc;
    }
    while (f_cnt <= n) {
        idx_t m = f_cnt;
        if (m == 0) {
            f_row[0] = 1;
        } else {
            f_row[m] = 0;
            for (idx_t j = m;; j--) {
                unsigned __int128 w = (unsigned __int128)(2 * j + 1);
                unsigned __int128 prev = j ? f_row[j - 1] : 0;
                f_row[j] = prev + w * w * f_row[j];
                if (j == 0) break;
            }
        }
        unsigned __int128 s = 0;
        for (idx_t j = 0; j <= m; j++) {
            if (j & 1) s -= f_row[j];
            else s += f_row[j];
        }
        f_res[m] = s;
        f_cnt++;
    }
}

static void big_extend(idx_t n) {
    if (n + 1 > b_cap) {
        idx_t nc = b_cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (b_cap == 0) {
            b_row = alloc_zero(nc);
            b_res = alloc_zero(nc);
        } else {
            b_row = grow_zero(b_row, b_cap, nc);
            b_res = grow_zero(b_res, b_cap, nc);
        }
        b_cap = nc;
    }
    while (b_cnt <= n) {
        idx_t m = b_cnt;
        if (m == 0) {
            b_row[0] = 1;
        } else {
            b_row[m] = 0;
            for (idx_t j = m;; j--) {
                idx_t w = 2 * j + 1;
                seq_t t = b_row[j] * (seq_t)(w * w);
                if (j) t += b_row[j - 1];
                b_row[j] = t;
                if (j == 0) break;
            }
        }
        seq_t s = 0;
        for (idx_t j = 0; j <= m; j++) {
            if (j & 1) s -= b_row[j];
            else s += b_row[j];
        }
        b_res[m] = s;
        b_cnt++;
    }
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        if (n >= f_cnt) fixed_extend(n);
        return from_i128((__int128)f_res[n]);
    }
    if (n >= b_cnt) big_extend(n);
    return b_res[n];
}
