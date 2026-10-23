/*
 * A010382  Squares mod 20.
 * https://oeis.org/A010382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = {0,1,4,5,9,16}[(n-1) mod 6]
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The six distinct squares modulo 20, sorted increasingly. */
    static const seq_t pattern[6] = {0, 1, 4, 5, 9, 16};

    /* n is guaranteed to be >= 1, so n-1 fits in idx_t. */
    idx_t idx = (n - 1) % 6;
    return pattern[idx];
}
