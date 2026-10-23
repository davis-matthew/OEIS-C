/*
 * A040649  Continued fraction for sqrt(675).
 * https://oeis.org/A040649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=25; for n>=1, a(n)=50 if n is even, otherwise 1
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the multiplicative function with a(2^e)=50 and a(p^e)=1 for odd primes. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)25;
    if ((n & (idx_t)1) == 0)   /* n even */
        return (seq_t)50;
    return (seq_t)1;
}
