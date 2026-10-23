/*
 * A109127  Expansion of q^(-1/8) * (eta(q^3) - eta(q)^3) / 3 in powers of q.
 * https://oeis.org/A109127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if 1+8n is not a perfect square; otherwise a(n)=(-1)^k
 *            * floor(2k/3) with k = (1+sqrt(1+8n))/2
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128.
   Returns floor(sqrt(x)). */
static inline unsigned __int128 isqrt(unsigned __int128 x)
{
    unsigned __int128 result = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126;  /* highest power of four <= 128-bit value */
    /* shift bit down until it is <= x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= result + bit) {
            x -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for values below offset */

    /* compute s = 1 + 8*n */
    unsigned __int128 s = (unsigned __int128)1 + (unsigned __int128)8 * n;

    unsigned __int128 d = isqrt(s);
    if (d * d != s) return 0;  /* n is not triangular */

    /* k = (1 + d) / 2 */
    unsigned long long k = (unsigned long long)((d + 1) >> 1);

    /* sign = (-1)^k */
    seq_t sign = (k & 1ULL) ? (seq_t)-1 : (seq_t)1;

    /* floor(2k/3) */
    seq_t val = (seq_t)((2ULL * (unsigned __int128)k) / 3ULL);

    return sign * val;
}
