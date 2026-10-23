/*
 * A181474  Sequence related to the Hankel transform of A105523(n+5).
 * https://oeis.org/A181474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+3)*(n+2)*(2*n^2+2*(-1)^n*n+10*n+5*(-1)^n+11)/96
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 671, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the polynomial part p = 2*n^2 + 2*(-1)^n*n + 10*n + 5*(-1)^n + 11 */
    int64_t n64 = (int64_t)n;
    int64_t p = 2 * n64 * n64;          /* 2*n^2 */
    if ((n64 & 1) == 0) {               /* (-1)^n = 1 */
        p += 2 * n64;   /* +2*n   */
        p += 5;         /* +5     */
    } else {                          /* (-1)^n = -1 */
        p -= 2 * n64;   /* -2*n   */
        p -= 5;         /* -5     */
    }
    p += 10 * n64 + 11;                /* +10*n + 11 */

    /* Multiply and divide using 128‑bit intermediate to avoid overflow */
    __int128 prod = (__int128)(n + 3) * (n + 2) * p;
    prod /= 96;

    return (seq_t)prod;
}
