/*
 * A140721  Alternated reading of A000302 and negated A002042.
 * https://oeis.org/A140721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less
 * Formula:   a(2n) = 4^n, a(2n+1) = -7 * 4^n
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute k = n/2 and 4^k */
    idx_t k   = n >> 1;          /* k = floor(n/2) */
    seq_t exponent = k;
    seq_t pow   = 1;             /* 4^0 = 1 */
    seq_t base  = 4;             /* base initial value */

    while (exponent > 0)
    {
        if (exponent & 1) pow *= base;
        base *= base;
        exponent >>= 1;
    }

    if (n & 1)                       /* odd index: multiply by -7 */
        pow *= (seq_t)-7;

    return pow;
}
