/*
 * A104586  Pentagonal wave sequence triangle.
 * https://oeis.org/A104586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) is obtained by arranging two “pentagonal wave” sequences
 *            in a triangular array. * Let r be the row number such that
 *            r(r‑1)/2 < n ≤ r(r+1)/2, and let * j = n – r(r‑1)/2 (1 ≤ j ≤ r).
 *            * Define i = r – j + 1. * If j is odd use the first-wave
 *            sequence: * A104584(i) = P(i) if i is odd * = P(i) +...
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static seq_t pentagonal(idx_t n)
{
    unsigned __int128 t = (unsigned __int128)n;
    unsigned __int128 val = t * ((unsigned __int128)3 * t - 1);
    val /= 2;
    return (seq_t)val;
}

static seq_t A104584(idx_t n)
{
    seq_t p = pentagonal(n);
    if ((n & 1) == 0) /* even n */
        p += (seq_t)n;
    return p;
}

static seq_t A104585(idx_t n)
{
    seq_t p = pentagonal(n);
    if ((n & 1) == 1) /* odd n */
        p += (seq_t)n;
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* defined only for n≥1 */
        return (seq_t)0;

    /* find row r:  r(r‑1)/2 < n ≤ r(r+1)/2  */
    long double d = sqrtl((long double)8 * n + 1);
    idx_t r = (idx_t)((d - 1) / 2);  /* floor of the quadratic solution */
    /* correct possible rounding down */
    unsigned __int128 tr = (unsigned __int128)r;
    if (tr * (tr + 1) / 2 < n)
        r++;

    /* column index j */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t j = n - (idx_t)prev;

    /* index used in the wave sequences */
    idx_t i = r - j + 1;

    if ((j & 1) == 1) /* odd column → A104584 */
        return A104584(i);
    else              /* even column → A104585 */
        return A104585(i);
}
