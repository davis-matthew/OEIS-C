/*
 * A212145  Number of (w,x,y,z) with all terms in {1,...,n} and w<2x+y+z.
 * https://oeis.org/A212145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3-3*(-1)^n-8*n-4*n^2+8*n^3+94*n^4)/96
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 119, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count of (w,x,y,z) with w,x,y,z in {1,…,n} and w < 2x + y + z. */
seq_t a(idx_t n)
{
    /* Convert index to seq_t to keep all arithmetic in the same type. */
    seq_t N = (seq_t)n;

    /* Positive part of the numerator: 6 if n is odd, plus 94*n^4 + 8*n^3. */
    seq_t pos = 0;
    if (n & 1)        /* odd n -> (-1)^n = -1, so 3-3*(-1)^n = 6 */
        pos += (seq_t)6;

    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;

    pos += (seq_t)94 * n4;    /* 94*n^4 */
    pos += (seq_t)8  * n3;    /* 8*n^3 */

    /* Negative part of the numerator: 8*n + 4*n^2. */
    seq_t neg = (seq_t)8 * N + (seq_t)4 * n2;  /* 8*n + 4*n^2 */

    /* Numerator = pos - neg.  It is guaranteed to be non‑negative. */
    seq_t num = pos - neg;

    /* Division by 96 gives the exact integer value of a(n). */
    return num / (seq_t)96;
}
