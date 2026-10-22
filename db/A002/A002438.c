/*
 * A002438  Multiples of Euler numbers.
 * https://oeis.org/A002438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000364(n-1) * (9^(n-1) + 1)/2, where A000364(m) = E(2m)
 *            is the secant (zigzag) number, taken from the Seidel-Entringer
 *            boustrophedon triangle: T(0,0) = 1, T(k,0) = 0, T(k,j) =
 *            T(k,j-1) + T(k-1,k-j), zigzag(k) = T(k,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A002438_MAXK 20004

static seq_t a002438_rA[A002438_MAXK + 1];
static seq_t a002438_rB[A002438_MAXK + 1];
static seq_t a002438_zig[A002438_MAXK + 1];
static seq_t *a002438_cur = 0;
static idx_t a002438_done = 0;

static seq_t a002438_zigzag(idx_t N) {
    if (!a002438_cur) {
        a002438_cur = a002438_rA;
        a002438_cur[0] = 1;
        a002438_zig[0] = 1;
        a002438_done = 0;
    }
    while (a002438_done < N) {
        seq_t *nx = (a002438_cur == a002438_rA) ? a002438_rB : a002438_rA;
        idx_t k = a002438_done + 1;
        idx_t j;
        nx[0] = 0;
        for (j = 1; j <= k; j++) {
            nx[j] = nx[j - 1];
            nx[j] += a002438_cur[k - j];
        }
        a002438_zig[k] = nx[k];
        a002438_cur = nx;
        a002438_done = k;
    }
    return a002438_zig[N];
}

seq_t a(idx_t n) {
    idx_t m, i;
    seq_t e, half, p;
    if (n == 0) return 0;
    m = n - 1;
    e = a002438_zigzag(2 * m);
    /* (9^m + 1)/2 = (9^m - 1)/2 + 1, avoiding overflow of 9^m + 1 */
    p = 1;
    for (i = 0; i < m; i++) p *= 9;
    half = (p - 1) / 2 + 1;
    return e * half;
}
