/*
 * A004098  Bell numbers written backwards.
 * https://oeis.org/A004098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(A000110(n)), i.e. the Bell number B(n) with its
 *            decimal digits reversed; B(n) is computed with the Bell (Aitken)
 *            triangle: T(0,0) = 1, T(i,0) = T(i-1,i-1), T(i,j) = T(i,j-1) +
 *            T(i-1,j-1), B(i) = T(i,0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *bt_row = 0;
static seq_t *bt_bell = 0;
static idx_t bt_cap = 0;
static idx_t bt_done = 0;

static seq_t reverse_digits(seq_t num) {
    seq_t result = 0;
    while (num > 0) {
        result = result * 10 + (num % 10);
        num /= 10;
    }
    return result;
}

static void ensure_cap(idx_t need) {
    if (need <= bt_cap) return;
    idx_t newcap = bt_cap * 2;
    if (newcap < need) newcap = need;
    if (newcap < 16) newcap = 16;
    seq_t *nr = alloc_zero(newcap);
    seq_t *nb = alloc_zero(newcap);
    for (idx_t i = 0; i < bt_done; i++) {
        nr[i] = bt_row[i];
        nb[i] = bt_bell[i];
    }
    if (bt_row) release(bt_row, bt_cap);
    if (bt_bell) release(bt_bell, bt_cap);
    bt_row = nr;
    bt_bell = nb;
    bt_cap = newcap;
}

static seq_t bell_number(idx_t n) {
    ensure_cap(n + 2);
    if (bt_done == 0) {
        bt_row[0] = 1;
        bt_bell[0] = 1;
        bt_done = 1;
    }
    while (bt_done <= n) {
        idx_t i = bt_done;           /* build row i (length i+1) from row i-1 */
        seq_t prev_old = bt_row[0];
        bt_row[0] = bt_row[i - 1];
        for (idx_t j = 1; j <= i; j++) {
            seq_t tmp = 0;
            if (j < i) tmp = bt_row[j];
            bt_row[j] = bt_row[j - 1] + prev_old;
            prev_old = tmp;
        }
        bt_bell[i] = bt_row[0];
        bt_done++;
    }
    return bt_bell[n];
}

seq_t a(idx_t n) {
    return reverse_digits(bell_number(n));
}
