/*
 * A000084  Number of series-parallel networks with n unlabeled edges. Also
 *          called yoke-chains by Cayley and MacMahon.
 * https://oeis.org/A000084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Product_{k>=1} 1/(1-x^k)^b(k) = 1 + Sum_{k>=1} a(k)*x^k, with b
 *            = A000669, b(1) = 1, b(n) = a(n)/2 for n >= 2; a(0) = 1, a(1) =
 *            1. Arbitrary precision: Euler transform recurrence n*a(n) =
 *            Sum_{k=1..n} d(k)*a(n-k), d(k) = Sum_{m|k} m*b(m), solved for
 *            a(n) (the k = n term contains n*a(n)/2). Fixed...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 39, u128 n <= 75,
 *            big any n
 * Helpers:   a84_gcd, a84_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a84_gcd.h"
#include "../utils/a84_small.h"

#define A84_MAXN 30000

static seq_t a84_A[A84_MAXN + 1];
static seq_t a84_B[A84_MAXN + 1];
static seq_t a84_D[A84_MAXN + 1];
static idx_t a84_top = 0;

static seq_t a84_big(idx_t n)
{
    if (a84_top == 0) {
        a84_A[0] = 1;
        a84_A[1] = 1;
        a84_B[1] = 1;
        a84_D[1] = 1;
        a84_top = 1;
    }
    while (a84_top < n) {
        idx_t m = a84_top + 1;
        seq_t s = 0;
        seq_t t;
        idx_t q, k;
        for (q = 1; q <= m / 2; q++)
            if (m % q == 0)
                s += a84_B[q] * (seq_t)q;
        t = s;
        for (k = 1; k < m; k++)
            t += a84_D[k] * a84_A[m - k];
        a84_A[m] = t * 2 / (seq_t)m;
        a84_B[m] = a84_A[m] / 2;
        a84_D[m] = s + a84_B[m] * (seq_t)m;
        a84_top = m;
    }
    return a84_A[n];
}

seq_t a(idx_t n)
{
    seq_t x = 1;
    int w = 0;
    while (w < 300 && x != 0) {
        x = x * 2;
        w++;
    }
    if (n == 0)
        return 1;
    if (w < 300) {
        /* fixed-width type: a(n) > 2^n >= 2^w for n > w, so it cannot fit */
        if (n > (idx_t)w || n > 128)
            return 0;
        return a84_small(n);
    }
    return a84_big(n);
}
