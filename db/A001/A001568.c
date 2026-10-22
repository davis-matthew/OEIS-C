/*
 * A001568  Related to 3-line Latin rectangles.
 * https://oeis.org/A001568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, more
 * Formula:   a(n) = Sum_{k=1..n-1} h(k) * ((n-1)!/k!) * S2(n-2,k-1) for n >=
 *            2, a(1) = 1, where S2 = Stirling numbers of the 2nd kind and
 *            h(k) = -h(k-1) - 2*(k-1)*h(k-2), h(0) = 1, h(1) = -1 (e.g.f.
 *            exp(-x-x^2)). Equivalently a(n) = (n-1)! * [n^-(n-1)] of the
 *            asymptotic series e^3 * K_n / n!^2 ~ Sum_k h(k) /...
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 10, i64 n <= 16, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A1568_MAX 10050

static seq_t a1568_h[A1568_MAX];
static seq_t a1568_S[A1568_MAX];
static idx_t a1568_hn = 0;

seq_t a(idx_t n)
{
    idx_t m, k, r;
    seq_t sum, P;

    if (n <= 1)
        return 1;
    if (n >= A1568_MAX)
        return 0;

    /* h(0..n-1), memoized */
    while (a1568_hn < n) {
        idx_t i = a1568_hn;
        if (i == 0)
            a1568_h[0] = 1;
        else if (i == 1)
            a1568_h[1] = -1;
        else
            a1568_h[i] = -a1568_h[i - 1] - a1568_h[i - 2] * (seq_t)(2 * (i - 1));
        a1568_hn++;
    }

    /* Stirling row S2(m, 0..m) */
    m = n - 2;
    a1568_S[0] = 1;
    for (k = 1; k <= m; k++)
        a1568_S[k] = 0;
    for (r = 1; r <= m; r++) {
        for (k = r; k >= 1; k--)
            a1568_S[k] = a1568_S[k] * (seq_t)k + a1568_S[k - 1];
        a1568_S[0] = 0;
    }

    /* sum over k, P = (n-1)!/k! */
    sum = 0;
    P = 1;
    for (k = n - 1; k >= 1; k--) {
        sum += a1568_h[k] * P * a1568_S[k - 1];
        P *= (seq_t)k;
    }
    return sum;
}
