/*
 * A006216  Number of down-up permutations of n+4 starting with 4.
 * https://oeis.org/A006216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*E(n+2) - E(n), where E(j) = A000111(j) are the Euler
 *            (up/down) numbers, computed with the Entringer/boustrophedon
 *            triangle T(0,0) = 1, T(m,0) = 0, T(m,k) = T(m,k-1) + T(m-1,m-k),
 *            E(m) = T(m,m)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *E_tab = NULL;
static seq_t *prev_row = NULL;
static seq_t *curr_row = NULL;
static idx_t cap = 0;
static idx_t done = 0;
static bool inited = false;

static void ensure(idx_t m) {
    if (!inited) {
        idx_t c = m + 1 < 16 ? 16 : m + 1;
        E_tab = alloc_zero(c);
        prev_row = alloc_zero(c);
        curr_row = alloc_zero(c);
        cap = c;
        E_tab[0] = 1;
        prev_row[0] = 1;
        done = 0;
        inited = true;
    }
    if (m + 1 > cap) {
        idx_t c = 2 * cap;
        if (c < m + 1) c = m + 1;
        E_tab = grow_zero(E_tab, cap, c);
        prev_row = grow_zero(prev_row, cap, c);
        curr_row = grow_zero(curr_row, cap, c);
        cap = c;
    }
    for (idx_t row = done + 1; row <= m; row++) {
        curr_row[0] = 0;
        for (idx_t k = 1; k <= row; k++) {
            curr_row[k] = curr_row[k - 1] + prev_row[row - k];
        }
        E_tab[row] = curr_row[row];
        seq_t *t = prev_row;
        prev_row = curr_row;
        curr_row = t;
        done = row;
    }
}

seq_t a(idx_t n) {
    ensure(n + 2);
    return 3 * E_tab[n + 2] - E_tab[n];
}
