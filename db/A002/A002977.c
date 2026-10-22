/*
 * A002977  Klarner-Rado sequence: a(1) = 1; subsequent terms are defined by
 *          the rule that if m is present so are 2m+1 and 3m+1.
 * https://oeis.org/A002977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(1) = 1; a(n) = min(2*a(i)+1, 3*a(j)+1) over the smallest
 *            values not yet emitted (two-pointer merge of the sorted streams
 *            2*a+1 and 3*a+1, duplicates removed)
 * Range:     u8 n <= 65, u16 n <= 4981, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"

static seq_t *kr_vals = NULL;
static idx_t kr_len = 0, kr_cap = 0;
static idx_t kr_i2 = 0, kr_i3 = 0;

seq_t a(idx_t n) {
    if (kr_len == 0) {
        kr_vals = A_grow(NULL, 0, 16);
        kr_cap = 16;
        kr_vals[0] = 1;
        kr_len = 1;
    }
    while (kr_len < n) {
        if (kr_len == kr_cap) {
            kr_vals = A_grow(kr_vals, kr_cap, kr_cap * 2);
            kr_cap *= 2;
        }
        seq_t x = kr_vals[kr_i2] * 2 + 1;
        seq_t y = kr_vals[kr_i3] * 3 + 1;
        seq_t m = (x < y) ? x : y;
        if (x == m) kr_i2++;
        if (y == m) kr_i3++;
        kr_vals[kr_len++] = m;
    }
    return kr_vals[n - 1];
}
