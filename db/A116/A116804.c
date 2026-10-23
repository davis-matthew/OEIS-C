/*
 * A116804  Number of permutations of length n which avoid the patterns 1234,
 *          1432, 4231.
 * https://oeis.org/A116804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (13*n^4 - 156*n^3 + 893*n^2 - 2790*n + 3840)/12 for n>=5;
 *            Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, return 0 */

    switch (n)
    {
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)6;
        case 4: return (seq_t)21;
        default:
            /* Use signed 64‑bit arithmetic for the numerator to avoid
               unsigned underflow in intermediate subtractions. */
            int64_t nn = (int64_t)n;
            int64_t n2 = nn * nn;          /* n^2 */
            int64_t n3 = n2 * nn;          /* n^3 */
            int64_t n4 = n3 * nn;          /* n^4 */
            int64_t num = 13 * n4
                         - 156 * n3
                         + 893 * n2
                         - 2790 * nn
                         + 3840;
            /* Division by 12 is exact for n >= 5 */
            return (seq_t)(num / 12);
    }
}
