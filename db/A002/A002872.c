/*
 * A002872  Number of partitions of {1..2n} that are invariant under a
 *          permutation consisting of n 2-cycles.
 * https://oeis.org/A002872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Aeven(n,k) = [n>0]*(k*Aeven(n-1,k) + Aeven(n-1,k-1) +
 *            Aeven(n-1,k-2)) + [n==0]*[k==0]; a(n) = Sum_{k=0..2n}
 *            Aeven(n,k), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A2872_MAX 20000

static seq_t a2872_row[2][2 * A2872_MAX + 3];
static seq_t a2872_res[A2872_MAX + 1];
static idx_t a2872_done = 0;
static int a2872_cur = 0;
static int a2872_inited = 0;

seq_t a(idx_t n) {
    if (n > A2872_MAX) return 0;
    if (!a2872_inited) {
        for (idx_t i = 0; i < 2 * A2872_MAX + 3; i++) {
            a2872_row[0][i] = 0;
            a2872_row[1][i] = 0;
        }
        a2872_row[0][0] = 1;
        a2872_res[0] = 1;
        a2872_done = 0;
        a2872_cur = 0;
        a2872_inited = 1;
    }
    while (a2872_done < n) {
        idx_t r = a2872_done + 1;
        seq_t *p = a2872_row[a2872_cur];
        seq_t *c = a2872_row[1 - a2872_cur];
        seq_t sum = 0;
        for (idx_t k = 0; k <= 2 * r; k++) {
            seq_t v = 0;
            if (k <= 2 * (r - 1)) v += (seq_t)k * p[k];
            if (k >= 1) v += p[k - 1];
            if (k >= 2) v += p[k - 2];
            c[k] = v;
            sum += v;
        }
        a2872_res[r] = sum;
        a2872_cur = 1 - a2872_cur;
        a2872_done = r;
    }
    return a2872_res[n];
}
