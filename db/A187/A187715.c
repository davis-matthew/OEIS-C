/*
 * A187715  a(n) = 5*n - (9 + (-1)^n)/2.
 * https://oeis.org/A187715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n-1) bitwise-OR 1 (equivalent to 5*n - (9 + (-1)^n)/2)
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 (offset 1) */
    seq_t term = 5 * ((seq_t)(n - 1));
    return term | 1;
}
