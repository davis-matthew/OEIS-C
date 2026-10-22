/*
 * A001934  Expansion of 1/theta_4(q)^2 in powers of q.
 * https://oeis.org/A001934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} b(k)*b(n-k), where b = A015128 =
 *            1/theta_4(q): b(0) = 1, b(n) = 2*Sum_{m>=1, m^2<=n} (-1)^(m+1) *
 *            b(n-m^2)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 143, u128 n <= 497,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A001934_CAP 65536

static seq_t a001934_b[A001934_CAP];
static idx_t a001934_blen = 0;

static void a001934_ensure(idx_t n) {
    idx_t i, m;
    while (a001934_blen <= n) {
        i = a001934_blen;
        if (i == 0) {
            a001934_b[0] = (seq_t)1;
        } else {
            seq_t pos = 0, neg = 0;
            for (m = 1; m * m <= i; m++) {
                if (m & 1) pos += a001934_b[i - m * m];
                else neg += a001934_b[i - m * m];
            }
            a001934_b[i] = (pos - neg) * 2;
        }
        a001934_blen++;
    }
}

seq_t a(idx_t n) {
    idx_t k;
    seq_t s = 0;
    if (n >= A001934_CAP) return (seq_t)0;
    a001934_ensure(n);
    for (k = 0; 2 * k < n; k++)
        s += a001934_b[k] * a001934_b[n - k];
    s = s * 2;
    if ((n & 1) == 0)
        s += a001934_b[n / 2] * a001934_b[n / 2];
    return s;
}
