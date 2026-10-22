/*
 * A000402  Number of permutations of [n] in which the longest increasing run
 *          has length 3.
 * https://oeis.org/A000402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B_3(n) - B_2(n), where B_k(n) is the number of
 *            permutations of [n] whose increasing runs all have length <= k.
 *            E.g.f. of B_k: 1/D_k(x), with D_k(x) = Sum_{j>=0}
 *            (x^(j(k+1))/(j(k+1))! - x^(j(k+1)+1)/(j(k+1)+1)!). So B_k(0) = 1
 *            and B_k(n) = Sum_{m=1..n, m==1 mod k+1} C(n,m)*B_k(n-m) -
 *            Sum_{m...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_grow.h"

static seq_t *B2 = 0, *B3 = 0, *row = 0;
static idx_t done = 0; /* B2[0..done-1], B3[0..done-1] computed; row = Pascal row done-1 */
static idx_t cap = 0;

static void extend(idx_t n) {
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < n + 1) nc *= 2;
        B2 = a306_grow(B2, cap, nc);
        B3 = a306_grow(B3, cap, nc);
        row = a306_grow(row, cap, nc);
        cap = nc;
    }
    while (done <= n) {
        idx_t j = done;
        /* build Pascal row j in place */
        row[j] = 1;
        if (j > 0) {
            idx_t i;
            for (i = j - 1; i >= 1; i--) row[i] += row[i - 1];
        }
        row[0] = 1;
        if (j == 0) {
            B2[0] = 1;
            B3[0] = 1;
        } else {
            seq_t p2 = 0, q2 = 0, p3 = 0, q3 = 0;
            idx_t m;
            for (m = 1; m <= j; m++) {
                idx_t r3 = m % 3, r4 = m % 4;
                if (r3 == 1) p2 += row[m] * B2[j - m];
                else if (r3 == 0) q2 += row[m] * B2[j - m];
                if (r4 == 1) p3 += row[m] * B3[j - m];
                else if (r4 == 0) q3 += row[m] * B3[j - m];
            }
            B2[j] = p2 - q2;
            B3[j] = p3 - q3;
        }
        done++;
    }
}

seq_t a(idx_t n) {
    extend(n);
    return B3[n] - B2[n];
}
