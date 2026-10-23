/*
 * A152205  Triangle read by rows, A000012 * A152204.
 * https://oeis.org/A152205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (r - 2*(c-1))^2, where r = ceil(sqrt(4*n) - 1) and c = n
 *            - floor(r^2/4)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Integer square root of an unsigned __int128 value.
   Returns floor(sqrt(x)).  Uses long double approximation and adjusts. */
static inline unsigned long long int_sqrt_u128(unsigned __int128 x)
{
    unsigned long long y = (unsigned long long)sqrtl((long double)x);
    while ((unsigned __int128)(y + 1) * (y + 1) <= x) ++y;
    while ((unsigned __int128)y * y > x) --y;
    return y;
}

seq_t a(idx_t n)
{
    /* Compute r = ceil(sqrt(4*n) - 1) */
    unsigned __int128 t = (unsigned __int128)4 * n;
    unsigned long long s = int_sqrt_u128(t);          /* floor(sqrt(t)) */
    unsigned long long r = (t == (unsigned __int128)s * s) ? s - 1 : s;

    /* Compute S_prev = floor(r^2 / 4) = sum of lengths of previous rows */
    unsigned __int128 r2 = (unsigned __int128)r * r;
    unsigned long long S_prev = (unsigned long long)(r2 / 4);

    /* Column index within the current row (1‑based) */
    idx_t c = n - (idx_t)S_prev;

    /* Value inside the triangle before squaring */
    unsigned long long f = r - 2 * (c - 1);

    /* Return the square as seq_t */
    seq_t fseq = (seq_t)f;
    return fseq * fseq;
}
