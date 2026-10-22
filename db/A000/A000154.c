/*
 * A000154  Erroneous version of A003713.
 * https://oeis.org/A000154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = Sum_{k=1..n} |Stirling1(n,k)|*(k-1)!, a(0) = 1 (e.g.f.
 *            -log(1+log(1-x)), as A003713), except for the erroneous term
 *            a(7) = 17382 that this dead sequence records in place of 17582.
 *            Computed as a(n) = Sum_{m=0..n-1} b(m)*(n-1)!/m!, where b =
 *            A007840: b(0) = 1, b(m) = Sum_{k=1..m} (m!/((m-k)!...
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A154_MAXN 20002

static seq_t a154_b[A154_MAXN];
static idx_t a154_have = 0;

static void a154_fill(idx_t m)
{
    while (a154_have <= m && a154_have < A154_MAXN) {
        idx_t i = a154_have;
        if (i == 0) {
            a154_b[0] = 1;
        } else {
            seq_t s = 0;
            seq_t f = 1;
            idx_t k;
            for (k = 1; k <= i; k++) {
                f *= (seq_t)(i - k + 1);
                s += f / (seq_t)k * a154_b[i - k];
            }
            a154_b[i] = s;
        }
        a154_have++;
    }
}

seq_t a(idx_t n)
{
    seq_t t;
    idx_t m, j;
    if (n == 0)
        return 1;
    if (n == 7)
        return 17382; /* erroneous term as printed in the sequence */
    m = n - 1;
    a154_fill(m);
    t = a154_b[0];
    for (j = 1; j <= m; j++)
        t = t * (seq_t)j + a154_b[j];
    return t;
}
