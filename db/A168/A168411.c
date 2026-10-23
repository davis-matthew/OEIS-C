/*
 * A168411  a(n) = 7 + 9*floor((n-1)/2).
 * https://oeis.org/A168411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 + 9 * floor((n-1)/2)
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t m = n - 1;                 /* m = n-1, nonnegative */
    seq_t k = (seq_t)(m / 2);        /* floor((n-1)/2) */
    return 7 + 9 * k;                /* 7 + 9 * k */
}
