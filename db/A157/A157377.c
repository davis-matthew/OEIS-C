/*
 * A157377  a(n) = 531441*n - 313146.
 * https://oeis.org/A157377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 531441*n - 313146
 * Range:     u8 none, u16 none, u32 n <= 8082, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)531441 - (seq_t)313146;
}
