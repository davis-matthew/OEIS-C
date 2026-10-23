/*
 * A108648  a(n) = (n+1)^2*(n+2)^3*(n+3)/24.
 * https://oeis.org/A108648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2*(n+2)^3*(n+3)/24
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 65, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n + 1;
    seq_t n2 = (seq_t)n + 2;
    seq_t n3 = (seq_t)n + 3;

    seq_t result = n1 * n1 * n2 * n2 * n2 * n3;
    result /= 24;

    return result;
}
