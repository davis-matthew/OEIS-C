/*
 * A005263  Number of labeled Greg trees.
 * https://oeis.org/A005263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   E.g.f. A(x) = 1 + B(x) - B(x)^2, B = e.g.f. of A005264, where
 *            1+2B = (1+x)*exp(B). Differentiating: (1+x)*(B - B^2)' = 2B + 1,
 *            giving with Q = B - B^2: Q(n+1) = 2*b(n) - n*Q(n), Q(1) = 1;
 *            b(n) = Q(n) + Sum_{i=1..n-1} binomial(n,i)*b(i)*b(n-i), b(1) =
 *            1; a(0) = 1, a(n) = Q(n) for n >= 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *bb = NULL;   /* b(n): A005264 */
static seq_t *qq = NULL;   /* Q(n) = a(n) for n >= 1 */
static seq_t *row = NULL;  /* binomial(cnt-1, i) */
static idx_t cnt = 0;      /* number of computed entries */
static idx_t cap = 0;

static void ensure_cap(idx_t need)
{
    idx_t nc;
    if (need <= cap) return;
    nc = cap ? cap : 8;
    while (nc < need) nc *= 2;
    bb = grow_zero(bb, cap, nc);
    qq = grow_zero(qq, cap, nc);
    row = grow_zero(row, cap, nc);
    cap = nc;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (cnt == 0) {
        ensure_cap(8);
        bb[0] = 0; qq[0] = 0;
        bb[1] = 1; qq[1] = 1;
        row[0] = 1; row[1] = 1;
        cnt = 2;
    }
    if (n >= cnt) {
        ensure_cap(n + 1);
        while (cnt <= n) {
            idx_t m = cnt, i;
            seq_t conv = 0;
            qq[m] = (seq_t)2 * bb[m - 1] - (seq_t)(m - 1) * qq[m - 1];
            for (i = m; i >= 1; i--)
                row[i] += row[i - 1];
            for (i = 1; 2 * i < m; i++)
                conv += row[i] * bb[i] * bb[m - i];
            conv *= 2;
            if (m % 2 == 0)
                conv += row[m / 2] * bb[m / 2] * bb[m / 2];
            bb[m] = qq[m] + conv;
            cnt++;
        }
    }
    return qq[n];
}
