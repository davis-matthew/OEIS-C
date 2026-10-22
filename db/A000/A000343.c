/*
 * A000343  5th power of rooted tree enumerator; number of linear forests of 5
 *          rooted trees.
 * https://oeis.org/A000343
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] B(x)^5, where B(x) = Sum_{m>=1} b(m) x^m is the
 *            g.f. of A000081: b(1) = 1, b(m) = (1/(m-1)) * Sum_{k=1..m-1}
 *            (Sum_{d|k} d*b(d)) * b(m-k). B^2, B^4 = (B^2)^2 and B^5 = B^4 *
 *            B are built by direct convolution
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 24, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A343_MAXN 12001

static seq_t A343_B[A343_MAXN];
static seq_t A343_S[A343_MAXN];
static seq_t A343_B2[A343_MAXN];
static seq_t A343_B4[A343_MAXN];
static seq_t A343_B5[A343_MAXN];
static idx_t a343_top = 0;

seq_t a(idx_t n) {
    if (n < 5 || n >= A343_MAXN) return 0;

    if (a343_top == 0) {
        A343_B[0] = 0; A343_B[1] = 1;
        A343_S[0] = 0; A343_S[1] = 1;
        A343_B2[0] = 0; A343_B2[1] = 0;
        A343_B4[0] = 0; A343_B4[1] = 0;
        A343_B5[0] = 0; A343_B5[1] = 0;
        a343_top = 1;
    }

    while (a343_top < n) {
        idx_t m = a343_top + 1;
        idx_t k, d, i;
        seq_t t;

        /* b(m) */
        t = 0;
        for (k = 1; k < m; k++)
            t += A343_S[k] * A343_B[m - k];
        A343_B[m] = t / (seq_t)(m - 1);

        /* S(m) = Sum_{d|m} d*b(d) */
        t = 0;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                t += (seq_t)d * A343_B[d];
                if (e != d) t += (seq_t)e * A343_B[e];
            }
        }
        A343_S[m] = t;

        /* B^2 */
        t = 0;
        for (i = 1; 2 * i < m; i++)
            t += A343_B[i] * A343_B[m - i];
        t += t;
        if (m % 2 == 0) t += A343_B[m / 2] * A343_B[m / 2];
        A343_B2[m] = t;

        /* B^4 */
        t = 0;
        for (i = 1; 2 * i < m; i++)
            t += A343_B2[i] * A343_B2[m - i];
        t += t;
        if (m % 2 == 0) t += A343_B2[m / 2] * A343_B2[m / 2];
        A343_B4[m] = t;

        /* B^5 */
        t = 0;
        for (i = 1; i < m; i++)
            t += A343_B4[i] * A343_B[m - i];
        A343_B5[m] = t;

        a343_top = m;
    }

    return A343_B5[n];
}
