/*
 * A002748  Sum of logarithmic numbers.
 * https://oeis.org/A002748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{m=0..n} (-1)^(n-m) * n!/(n-m)! * sigma(m+1), from
 *            E.g.f. F(x)/exp(x)/x with F the o.g.f. of A000203; evaluated by
 *            Horner's rule
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t N = n + 1;
    uint64_t *sg = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    idx_t d, k;
    for (d = 1; d <= N; d++)
        for (k = d; k <= N; k += d)
            sg[k] += d;

    /* r = c_n; for m = n-1 down to 0: r = c_m + (n-m)*r, with c_m = (-1)^(n-m) sigma(m+1) */
    seq_t r = (seq_t)sg[n + 1];
    idx_t m = n;
    while (m > 0) {
        m--;
        seq_t s = (seq_t)sg[m + 1];
        seq_t t = r * (seq_t)(n - m);
        if ((n - m) & 1)
            r = t - s;
        else
            r = t + s;
    }
    free(sg);
    return r;
}
