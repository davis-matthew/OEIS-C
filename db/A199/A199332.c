/*
 * A199332  Triangle read by rows, where even numbered rows contain the
 *          nonsquares (cf. A000037) and odd rows contain replicated squares.
 * https://oeis.org/A199332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + the nth nonsquare if the row of n is even, otherwise
 *            a square ((row+1)/2)^2. Number of nonsquares used before row r
 *            (even) equals f*(f+1), where f = floor((r-1)/2)
 * Range:     u8 n <= 465, u16 n <= 11325, u32 n <= 11325, u64 n <= 11325,
 *            u128 n <= 11325, big any n
 * Validated: standard, pass, 84 terms through n = 7948 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

/* Return the m‑th nonsquare integer (m >= 1). */
static seq_t nth_nonsquare(idx_t m)
{
    /* Find s such that s^2 - s <= m <= s^2 + s. */
    idx_t s = isqrt(m);
    while ((unsigned __int128)s * s - s > m) s--;
    while (m > (unsigned __int128)s * s + s) s++;

    return (seq_t)(m + s);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */

    /* Find the row r such that T(r-1) < n <= T(r). */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n) r++;

    /* Elements before this row */
    idx_t prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t offset   = n - prev_tri - 1;   /* 0‑based position in the row */

    if ((r & 1) == 1)   /* odd row – replicated square */
    {
        seq_t k = (seq_t)(r + 1) / 2;
        return k * k;
    }
    else                /* even row – nonsquares */
    {
        idx_t f        = (r - 1) / 2;           /* number of even rows before r */
        idx_t prev_ns  = f * (f + 1);           /* nonsquares used before r */
        idx_t m        = prev_ns + offset + 1;  /* index among nonsquares */
        return nth_nonsquare(m);
    }
}
