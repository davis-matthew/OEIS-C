/*
 * A070153  Take pairs (x,y) with Sum_{j = x..y} j = concatenation of x and y.
 *          Sort pairs on y then x. This sequence gives y of each pair.
 * https://oeis.org/A070153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = y where there exists an integer x ≥ 1 such that Σ_{j=x}^y
 *            j = concat(x, y). The function iterates y, determining the
 *            interval length via the quadratic discriminant, and stores the
 *            valid y values in order
 * Range:     u8 none, u16 n <= 5, u32 n <= 21, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digits_seq, floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt_seq.h"
#include "../utils/digits_seq.h"

static seq_t *y_vals = NULL;      /* stored y values */
static size_t  y_count = 0;      /* number of stored values   */
static size_t  y_cap   = 0;      /* allocated capacity       */
static seq_t   last_y  = 0;      /* last y examined          */

/* Ensure the buffer has space for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (y_cap >= need) return;
    size_t new_cap = y_cap ? y_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(y_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    y_vals = tmp;
    y_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* ignore offset 0 */

    while (y_count < (size_t)n) {
        last_y++;
        seq_t y = last_y;
        if (y == 0) continue;    /* safety, never happens */

        /* number of decimal digits of y */
        int d = digits_seq(y);

        /* P = 10^d */
        seq_t P = 1;
        for (int i = 0; i < d; ++i) P *= 10;

        /* Quadratic coefficients */
        seq_t A = (y + P) * 2 + 1;                      /* 2y+2P+1 */
        seq_t B = (P * y + P + y) * 2;                  /* 2Py+2P+2y */

        /* Discriminant D = A^2 - 4B */
        seq_t D = A * A - 4 * B;

        seq_t sqrtD = floor_sqrt_seq(D);
        if (sqrtD * sqrtD != D) continue;   /* not a perfect square */

        seq_t t = (A - sqrtD) / 2;           /* interval length = y - x + 1 */
        if (t == 0) continue;

        seq_t x = y - t + 1;
        if (x < 1) continue;

        seq_t sum    = ((x + y) * t) / 2;
        seq_t concat = x * P + y;
        if (sum != concat) continue;

        /* Record the valid y. */
        ensure_capacity(y_count + 1);
        y_vals[y_count++] = y;
    }

    return y_vals[n - 1];
}
