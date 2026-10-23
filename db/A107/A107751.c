/*
 * A107751  a(n) = A107750(n+1) - A107750(n).
 * https://oeis.org/A107751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 2 or n mod 3 < 2, otherwise 2
 * Range:     u8 n <= 9968, u16 n <= 9968, u32 n <= 9968, u64 n <= 9968,
 *            u128 n <= 9968, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2ULL)
        return (seq_t)1;
    if ((n % 3ULL) < 2ULL)
        return (seq_t)1;
    return (seq_t)2;
}
