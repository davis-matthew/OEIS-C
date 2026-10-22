/*
 * A005003  Number of rhyme schemes (see reference for precise definition).
 * https://oeis.org/A005003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + A000110(n+1) - 1, a(1) = 1 (OEIS: a(k)=1,
 *            a(n)=k*a(n-1)+A000110(n-1)-A102661(n-1,k-2), k=3, reindexed to
 *            offset 1). Bell numbers are produced incrementally with the Bell
 *            triangle
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *av = NULL;   /* av[1..N] computed terms */
static idx_t avcap = 0, N = 0;
static seq_t *row = NULL;  /* current Bell triangle row R, length R+1 */
static idx_t rowcap = 0, R = 0;

static void ensure(seq_t **arr, idx_t *cap, idx_t need) {
    if (*cap >= need) return;
    idx_t nc = need > 2 * (*cap) ? need : 2 * (*cap);
    if (*cap == 0) *arr = alloc_zero(nc);
    else *arr = grow_zero(*arr, *cap, nc);
    *cap = nc;
}

/* Advance the Bell triangle by one row, in place. */
static void advance_row(void) {
    idx_t r = R + 1;
    seq_t prev = row[0];
    row[0] = row[r - 1];
    for (idx_t j = 1; j <= r; j++) {
        seq_t t = row[j];
        row[j] = row[j - 1] + prev;
        prev = t;
    }
    R = r;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure(&av, &avcap, n + 1);
    ensure(&row, &rowcap, n + 2);
    if (N == 0) {
        av[1] = 1;
        row[0] = 1;
        R = 0;
        N = 1;
    }
    while (N < n) {
        idx_t m = N + 1;
        while (R < m) advance_row();
        /* last element of row m is Bell(m+1) */
        av[m] = (seq_t)3 * av[m - 1] + row[m] - 1;
        N = m;
    }
    return av[n];
}
