/*
 * A000182  Tangent (or "Zag") numbers: e.g.f. tan(x), also (up to signs)
 *          e.g.f. tanh(x).
 * https://oeis.org/A000182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = E(2n-1), the zigzag (Euler up/down) number, computed with
 *            the Seidel-Entringer boustrophedon triangle: T(0,0) = 1, T(m,0)
 *            = 0 for m >= 1, T(m,k) = T(m,k-1) + T(m-1,m-k), E(m) = T(m,m).
 *            a(n) = T(2n-1,2n-1). Only additions are used, so fixed-width
 *            results are exact mod 2^w
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A182_MAXN 10000
#define A182_ROWS (2 * A182_MAXN + 2)

static seq_t a182_bufA[A182_ROWS];
static seq_t a182_bufB[A182_ROWS];
static seq_t a182_res[A182_MAXN + 1];
static seq_t *a182_prev = a182_bufA;
static seq_t *a182_cur = a182_bufB;
static idx_t a182_m = 0;     /* index of the row held in a182_prev */
static idx_t a182_done = 0;  /* a(1..a182_done) are stored in a182_res */
static bool a182_init = false;

seq_t a(idx_t n)
{
    if (!a182_init) {
        a182_prev[0] = 1;
        a182_init = true;
    }
    if (n < 1 || n > A182_MAXN)
        return 0;
    while (a182_done < n) {
        idx_t m = ++a182_m;
        idx_t k;
        seq_t *t;
        a182_cur[0] = 0;
        for (k = 1; k <= m; k++)
            a182_cur[k] = a182_cur[k - 1] + a182_prev[m - k];
        t = a182_prev;
        a182_prev = a182_cur;
        a182_cur = t;
        if (m & 1) {
            a182_done = (m + 1) / 2;
            a182_res[a182_done] = a182_prev[m];
        }
    }
    return a182_res[n];
}
