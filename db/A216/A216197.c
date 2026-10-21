/*
 * A216197  Abelian complexity function of A064990.
 * https://oeis.org/A216197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2; a(3n) = a(n)+2; a(3n-1) = a(n)+1; a(3n+1) = a(n)+1
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if ((n % 3) == 0)
        return a(n / 3) + (seq_t)2;
    else
        return a((n + 1) / 3) + (seq_t)1;
}
