/*
 * A109354  a(n) = 6^((n^2 - n)/2).
 * https://oeis.org/A109354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^((n^2-n)/2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms: a(0) = a(1) = 1 */
    if (n < 2)
        return 1;

    /* Compute the exponent exp = n*(n-1)/2 using idx_t arithmetic. */
    idx_t exp = n * (n - 1) / 2;

    /* Fast exponentiation by squaring.  seq_t supports multiplication. */
    seq_t result = 1;
    seq_t base   = 6;

    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return result;
}
