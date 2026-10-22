/*
 * A001247  Squares of Bell numbers.
 * https://oeis.org/A001247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell(n)^2, where Bell(n) comes from the Bell triangle:
 *            T(0,0) = 1, T(i,0) = T(i-1,i-1), T(i,j) = T(i,j-1) + T(i-1,j-1),
 *            Bell(i) = T(i,0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   a001028_grow, a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001028_z.h"
#include "../utils/a001028_grow.h"

static seq_t *a001247_row = 0;   /* current Bell-triangle row */
static seq_t *a001247_bell = 0;  /* cached Bell numbers */
static idx_t a001247_len = 0;    /* number of cached Bell numbers */
static idx_t a001247_cap = 0;

seq_t a(idx_t n)
{
    if (n >= a001247_len) {
        if (n + 1 > a001247_cap) {
            idx_t nc = a001247_cap ? a001247_cap : 16;
            while (nc < n + 1)
                nc *= 2;
            if (a001247_cap == 0) {
                a001247_row = a001028_z(nc);
                a001247_bell = a001028_z(nc);
            } else {
                a001247_row = a001028_grow(a001247_row, a001247_cap, nc);
                a001247_bell = a001028_grow(a001247_bell, a001247_cap, nc);
            }
            a001247_cap = nc;
        }
        if (a001247_len == 0) {
            a001247_row[0] = 1;
            a001247_bell[0] = 1;
            a001247_len = 1;
        }
        for (idx_t i = a001247_len; i <= n; i++) {
            /* transform row i-1 (entries 0..i-1) into row i (entries 0..i) in place */
            seq_t prev = a001247_row[0];
            a001247_row[0] = a001247_row[i - 1];
            for (idx_t j = 1; j <= i; j++) {
                seq_t t = a001247_row[j];
                a001247_row[j] = a001247_row[j - 1] + prev;
                prev = t;
            }
            a001247_bell[i] = a001247_row[0];
        }
        a001247_len = n + 1;
    }
    return a001247_bell[n] * a001247_bell[n];
}
