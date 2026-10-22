/*
 * A000110  Bell or exponential numbers: number of ways to partition a set of
 *          n labeled elements.
 * https://oeis.org/A000110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, nonn, easy, nice, changed
 * Formula:   Bell triangle (Aitken's array): row 0 = [1]; row i starts with
 *            the last entry of row i-1, and each following entry = (entry to
 *            its left) + (entry above-left). a(n) = first entry of row n,
 *            a(0) = 1. Equivalent to a(n+1) = Sum_{k=0..n} a(k)*binomial(n,
 *            k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_series.h"

static seq_t *bell_row = 0;   /* current row of the Bell triangle */
static seq_t *bell_val = 0;   /* bell_val[i] = a(i) for i <= bell_top */
static size_t bell_cap = 0;
static idx_t bell_top = 0;    /* index of the current row */
static int bell_init = 0;

static void bell_grow(size_t need)
{
    size_t cap = bell_cap ? bell_cap : 64;
    size_t i;
    seq_t *nr, *nv;
    while (cap < need) cap *= 2;
    nr = alloc_series(cap);
    nv = alloc_series(cap);
    for (i = 0; i < bell_cap; i++) {
        nr[i] = bell_row[i];
        nv[i] = bell_val[i];
    }
    bell_row = nr;
    bell_val = nv;
    bell_cap = cap;
}

seq_t a(idx_t n)
{
    if (!bell_init) {
        bell_grow(64);
        bell_row[0] = 1;
        bell_val[0] = 1;
        bell_top = 0;
        bell_init = 1;
    }
    if (n > bell_top && (size_t)n + 2 > bell_cap)
        bell_grow((size_t)n + 2);
    while (bell_top < n) {
        /* current row has bell_top+1 entries; build next row in place */
        size_t len = (size_t)bell_top + 1;
        size_t j;
        seq_t above = bell_row[0];
        seq_t tmp;
        bell_row[0] = bell_row[len - 1];
        for (j = 1; j <= len; j++) {
            if (j < len) tmp = bell_row[j];
            bell_row[j] = bell_row[j - 1] + above;
            if (j < len) above = tmp;
        }
        bell_top++;
        bell_val[bell_top] = bell_row[0];
    }
    return bell_val[n];
}
