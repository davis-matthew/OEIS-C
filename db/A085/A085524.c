/*
 * A085524  a(0) = 0; a(n) = n^(2*n-1) for n > 0.
 * https://oeis.org/A085524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n^(2*n-1) for n > 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t base = (seq_t)n;
    idx_t  exp   = 2*n - 1;   /* 2*n-1 is safe for n up to 10000 */
    seq_t  result = 1;

    while (exp) {
        if (exp & 1)          /* if current bit is set, multiply result */
            result *= base;
        exp >>= 1;            /* shift to next bit */
        if (exp)               /* square base only if more bits remain */
            base *= base;
    }
    return result;
}
