/*
 * A091685  Sieve out 6n+1 and 6n-1.
 * https://oeis.org/A091685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = 0 if n == 0 or n is divisible by 2 or 3; a(n) = n
 *            otherwise. (Completely multiplicative, a(2)=a(3)=0, a(p)=p for
 *            p>3)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if ((n & 1) == 0 || n % 3 == 0) return 0;
    return (seq_t)n;
}
