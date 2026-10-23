/*
 * A101867  Main diagonal of A101866.
 * https://oeis.org/A101867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + ceil(n*(1 + sqrt(5))/2)^2
 * Range:     u8 n <= 7, u16 n <= 133, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

static inline seq_t ceil_phi(idx_t n)
{
    /* m = floor(n * sqrt(5)) = floor(sqrt(5 * n^2)) */
    seq_t nn   = (seq_t)n * (seq_t)n;
    seq_t t    = (seq_t)5 * nn;
    seq_t m    = isqrt(t);
    seq_t sum  = (seq_t)n + m;
    /* ceil(n*phi) = floor((n+m)/2) + 1 */
    return (sum >> 1) + 1;
}

seq_t a(idx_t n)
{
    seq_t n2   = (seq_t)n * (seq_t)n;
    seq_t c    = ceil_phi(n);
    seq_t c2   = c * c;
    return n2 + c2;
}
