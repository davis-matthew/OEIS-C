/*
 * A005001  a(n) = Sum_{k=0..n-1} Bell(k), where the Bell numbers Bell(k) are
 *          given in A000110.
 * https://oeis.org/A005001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n-1} Bell(k), a(0) = 0; Bell(k) computed via
 *            the Bell triangle (row k starts with Bell(k))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *row = NULL;   /* current Bell triangle row, length rowlen */
static idx_t rowlen = 0;    /* row index r has length r+1 */
static seq_t *sums = NULL;  /* sums[m] = Sum_{k=0..m-1} Bell(k) */
static idx_t nsums = 0;     /* number of valid entries in sums */
static idx_t cap = 0;

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    seq_t *nr = alloc_zero(nc);
    seq_t *ns = alloc_zero(nc);
    for (idx_t i = 0; i < rowlen; i++) nr[i] = row[i];
    for (idx_t i = 0; i < nsums; i++) ns[i] = sums[i];
    if (row) release(row, cap);
    if (sums) release(sums, cap);
    row = nr;
    sums = ns;
    cap = nc;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (nsums == 0) {
        ensure_cap(2);
        row[0] = 1;      /* row 0: Bell(0) = 1 */
        rowlen = 1;
        sums[0] = 0;
        sums[1] = 1;
        nsums = 2;
    }
    while (nsums <= n) {
        /* build next row of the Bell triangle in place */
        idx_t i = rowlen;  /* new row index, new length i+1 */
        ensure_cap(i + 2);
        seq_t old = row[0];
        row[0] = row[i - 1];
        for (idx_t j = 1; j <= i; j++) {
            seq_t nxt = row[j];
            row[j] = row[j - 1] + old;
            old = nxt;
        }
        rowlen = i + 1;
        /* row[0] is Bell(i) */
        sums[nsums] = sums[nsums - 1] + row[0];
        nsums++;
    }
    return sums[n];
}
