/*
 * A081913  a(n) = 2^n*(n^3 - 3*n^2 + 2*n + 48)/48.
 * https://oeis.org/A081913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n*(n^3 - 3*n^2 + 2*n + 48)/48
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 52, u128 n <= 113,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* For n < 4 we can safely compute the product before the division. */
    if (n < 4) {
        seq_t poly = nseq * nseq * nseq - 3 * nseq * nseq + 2 * nseq + 48;
        seq_t pow2n = pow2(n);
        return (pow2n * poly) / (seq_t)48;
    }

    /* For n >= 4, factor out 16 from 2^n (since 48 = 16*3) to avoid overflow. */
    seq_t poly = nseq * nseq * nseq - 3 * nseq * nseq + 2 * nseq + 48;
    seq_t poly3 = poly / 3;          /* poly is always divisible by 3 */
    seq_t pow2shift = pow2(n - 4);   /* equals 2^(n-4) */
    return pow2shift * poly3;
}
