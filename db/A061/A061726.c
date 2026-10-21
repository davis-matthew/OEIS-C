/*
 * A061726  If n-th triangular number (A000217(n)) is odd, multiply it by 4;
 *          if even, multiply it by 5.
 * https://oeis.org/A061726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n*(n+1)/2) * (5 - ((n*(n+1)/2) mod 2))
 * Range:     u8 n <= 10, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns   = (seq_t)n;
    seq_t tri  = ns * (ns + 1) / 2;          /* n-th triangular number */
    seq_t mult = (seq_t)5 - (tri & 1);       /* 4 if tri odd, 5 if tri even */
    return tri * mult;
}
