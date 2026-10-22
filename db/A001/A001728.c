/*
 * A001728  Expansion of cos x / cos 4x.
 * https://oeis.org/A001728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{j=1..n} (-1)^(j+1) * C(2n,2j) * 16^j *
 *            a(n-j), a(0) = 1 (from cos(x) = (Sum_n a(n) x^(2n)/(2n)!) *
 *            cos(4x))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   a001028_grow, a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001028_z.h"
#include "../utils/a001028_grow.h"

static seq_t *A = NULL;    /* A[i] = a(i) */
static seq_t *pw = NULL;   /* pw[j] = 16^j */
static seq_t *row = NULL;  /* current Pascal row, index m */
static idx_t cap = 0;      /* capacity of A and pw */
static idx_t cnt = 0;      /* number of terms computed */
static idx_t m = 0;        /* current Pascal row number */

static void ensure(idx_t need) {
    idx_t newcap;
    if (need <= cap) return;
    newcap = cap * 2;
    if (newcap < need) newcap = need;
    if (newcap < 16) newcap = 16;
    if (A == NULL) {
        A = a001028_z(newcap);
        pw = a001028_z(newcap);
        row = a001028_z(2 * newcap + 1);
    } else {
        A = a001028_grow(A, cap, newcap);
        pw = a001028_grow(pw, cap, newcap);
        row = a001028_grow(row, 2 * cap + 1, 2 * newcap + 1);
    }
    cap = newcap;
}

static void pascal_step(void) {
    idx_t k;
    m++;
    row[m] = 1;
    for (k = m - 1; k >= 1; k--)
        row[k] += row[k - 1];
}

seq_t a(idx_t n) {
    idx_t i, j;
    ensure(n + 1);
    if (cnt == 0) {
        A[0] = 1;
        pw[0] = 1;
        row[0] = 1;
        m = 0;
        cnt = 1;
    }
    for (i = cnt; i <= n; i++) {
        seq_t pos, neg;
        pw[i] = pw[i - 1] * 16;
        pascal_step();
        pascal_step();            /* row now holds C(2i, k) */
        pos = (i % 2 == 0) ? 1 : 0;
        neg = (i % 2 == 1) ? 1 : 0;
        for (j = 1; j <= i; j++) {
            seq_t term = row[2 * j] * pw[j] * A[i - j];
            if (j % 2 == 1)
                pos += term;
            else
                neg += term;
        }
        A[i] = pos - neg;
        cnt = i + 1;
    }
    return A[n];
}
