/*
 * A091435  Array T(n,k) = n*(n+k), read by antidiagonals.
 * https://oeis.org/A091435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Validated: standard, pass, 94 terms through n = 5002 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A091435
    Table T(n,k) = n*(n+k) (n,k ≥ 0) read by antidiagonals (row-first).
    The index n corresponds to a pair (r,c) on diagonal d = r+c:
        d = floor((sqrt(8*n+1)-1)/2)
        p = n - d*(d+1)/2          // position on this diagonal
        r = d - p
        c = p
    Then a(n) = T(r,c) = r*(r+c) = r*d.
*/
#include <math.h>   /* sqrtl */

static inline idx_t find_diagonal(idx_t n)
{
    /* solve d(d+1)/2 ≤ n < (d+1)(d+2)/2   (d is the diagonal index) */
    long double d_est = (sqrtl((long double)n * 8.0L + 1.0L) - 1.0L) / 2.0L;
    idx_t d = (idx_t)d_est;                     /* floor of estimate */

    /* correct possible floor/ceil errors using 128‑bit arithmetic */
    while ((unsigned __int128)d * (d + 1) / 2 > n) --d;
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= n) ++d;
    return d;
}

seq_t a(idx_t n)
{
    /* compute diagonal and offset */
    idx_t d = find_diagonal(n);
    unsigned __int128 prev = (unsigned __int128)d * (d + 1) / 2;   /* terms before this diagonal */
    idx_t p = (idx_t)(n - prev);                                 /* position on the diagonal (0…d) */

    /* row (r) and column (c) */
    idx_t r = d - p;
    /* c = p; (not needed for computation) */

    /* final value a(n) = r * d */
    return (seq_t)r * (seq_t)d;
}
