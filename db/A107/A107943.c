/*
 * A107943  a(n) = (n+1)*(n+2)^2*(n+3)^2*(n+4)^2*(n+5)*(2n+3)/8640.
 * https://oeis.org/A107943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(n+2)^2*(n+3)^2*(n+4)^2*(n+5)*(2n+3) / 8640
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes the n-th term of OEIS sequence A107943 */
seq_t a(idx_t n)
{
    /* Use 128‑bit integer for intermediate arithmetic to avoid overflow
       on 64‑bit builds.  The final result always fits in the target
       seq_t because the arithmetic is exact and the division by 8640
       guarantees an integer result. */
    unsigned __int128 prod = 1;

    prod *= (unsigned __int128)(n + 1);

    /* (n+2)^2 */
    prod *= (unsigned __int128)(n + 2);
    prod *= (unsigned __int128)(n + 2);

    /* (n+3)^2 */
    prod *= (unsigned __int128)(n + 3);
    prod *= (unsigned __int128)(n + 3);

    /* (n+4)^2 */
    prod *= (unsigned __int128)(n + 4);
    prod *= (unsigned __int128)(n + 4);

    prod *= (unsigned __int128)(n + 5);

    /* (2n+3) */
    prod *= (unsigned __int128)(2 * n + 3);

    /* Divide by 8640 = 2^6 * 3^3 * 5 */
    prod /= 8640u;

    return (seq_t)prod;
}
