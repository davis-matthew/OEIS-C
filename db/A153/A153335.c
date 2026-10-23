/*
 * A153335  Number of zig-zag paths from top to bottom of an n X n square
 *          whose color is not that of the top right corner.
 * https://oeis.org/A153335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*2^(n-2) - 2*(n-1)*C(n-2,(n-2)/2) for even n; a(n) =
 *            (n+1)*2^(n-2) - n*C(n-1,(n-1)/2) for odd n
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)          /* offset is 1; a(1)=0 */
        return (seq_t)0;

    /* part1 = (n+1) * 2^(n-2) */
    seq_t part1 = (seq_t)(n + 1) * pow2(n - 2);

    if ((n & 1) == 0) {           /* even n */
        seq_t bin = binomial(n - 2, (n - 2) / 2);
        idx_t mult = 2 * (n - 1); /* 2*(n-1) fits in idx_t for n ≤ 10000 */
        seq_t part2 = bin * (seq_t)mult;
        return part1 - part2;
    } else {                       /* odd n */
        seq_t bin = binomial(n - 1, (n - 1) / 2);
        idx_t mult = n;           /* n fits in idx_t */
        seq_t part2 = bin * (seq_t)mult;
        return part1 - part2;
    }
}
