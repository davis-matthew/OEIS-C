/*
 * A004404  Expansion of 1 / (Sum_{n=-oo..oo} x^(n^2))^3.
 * https://oeis.org/A004404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. 1/theta_3(x)^3, where theta_3(x) = 1 + 2*Sum_{k>=1}
 *            x^(k^2). It is done as three inversions by theta_3. Each one
 *            uses the sparse recurrence y(m) = x(m) - 2*Sum_{k>=1, k^2<=m}
 *            y(m-k^2): b1 = 1/theta_3 (with x = [m==0]), b2 = b1/theta_3, a =
 *            b2/theta_3
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 29, i64 n <= 97, i128 n <= 336,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A004404_CAP 32768

static seq_t A004404_b1[A004404_CAP];
static seq_t A004404_b2[A004404_CAP];
static seq_t A004404_c[A004404_CAP];
static idx_t A004404_done = 0;

seq_t a(idx_t n) {
    if (n >= (idx_t)A004404_CAP) return 0;
    while (A004404_done <= n) {
        idx_t m = A004404_done;
        idx_t j;
        seq_t s1 = 0, s2, s3;
        if (m == 0) s1 = 1;
        for (j = 1; j * j <= m; j++) {
            s1 -= A004404_b1[m - j * j];
            s1 -= A004404_b1[m - j * j];
        }
        A004404_b1[m] = s1;
        s2 = A004404_b1[m];
        for (j = 1; j * j <= m; j++) {
            s2 -= A004404_b2[m - j * j];
            s2 -= A004404_b2[m - j * j];
        }
        A004404_b2[m] = s2;
        s3 = A004404_b2[m];
        for (j = 1; j * j <= m; j++) {
            s3 -= A004404_c[m - j * j];
            s3 -= A004404_c[m - j * j];
        }
        A004404_c[m] = s3;
        A004404_done++;
    }
    return A004404_c[n];
}
