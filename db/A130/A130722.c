/*
 * A130722  The twice repeated nonnegative integers at even indices, the
 *          non-repeated nonnegative integers at odd indices.
 * https://oeis.org/A130722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = floor(n/2), a(2*n+1) = n. Equivalent: if n is even,
 *            a(n) = n/4; if n is odd, a(n) = n/2
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)         /* even index */
        return (seq_t)(n / 4); /* floor((n/2)/2) */
    else                      /* odd index  */
        return (seq_t)(n / 2); /* n/2  */
}
