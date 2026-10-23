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
 * Range:     u8 n <= 465, u16 n <= 7948, u32 n <= 7948, u64 n <= 7948,
 *            u128 n <= 7948, big any n
 * Helpers:   int_u64_isqrt, nth_nonsquare
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_u64_isqrt.h"
#include "../utils/nth_nonsquare.h"

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
