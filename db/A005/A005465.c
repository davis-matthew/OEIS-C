/*
 * A005465  Number of n-dimensional hypotheses allowing for conditional
 *          independence.
 * https://oeis.org/A005465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Bell(n+2) - Bell(n+1) - 3^n, with Bell numbers from the
 *            Bell triangle
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

static seq_t *bt_row = 0;   /* current row of the Bell triangle */
static seq_t *bt_bell = 0;  /* Bell numbers computed so far */
static idx_t bt_cap = 0;
static idx_t bt_r = 0;      /* index of current row (has bt_r+1 entries) */

seq_t a(idx_t n) {
    idx_t need = n + 3;
    if (bt_cap == 0) {
        idx_t c = need < 16 ? 16 : need;
        bt_row = grow_zero(0, 0, c);
        bt_bell = grow_zero(0, 0, c);
        bt_cap = c;
        bt_row[0] = 1;
        bt_bell[0] = 1;
        bt_r = 0;
    } else if (bt_cap < need) {
        idx_t c = bt_cap * 2;
        if (c < need) c = need;
        bt_row = grow_zero(bt_row, bt_cap, c);
        bt_bell = grow_zero(bt_bell, bt_cap, c);
        bt_cap = c;
    }
    while (bt_r < n + 2) {
        seq_t prev = bt_row[0];
        bt_row[0] = bt_row[bt_r];
        for (idx_t j = 1; j <= bt_r + 1; j++) {
            seq_t tmp = bt_row[j];
            bt_row[j] = bt_row[j - 1] + prev;
            prev = tmp;
        }
        bt_r++;
        bt_bell[bt_r] = bt_row[0];
    }
    seq_t p = 1;
    for (idx_t i = 0; i < n; i++) p *= 3;
    seq_t result = bt_bell[n + 2] - bt_bell[n + 1];
    result -= p;
    return result;
}
