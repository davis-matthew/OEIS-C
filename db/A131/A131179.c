/*
 * A131179  a(n) = if n mod 2 == 0 then n*(n+1)/2, otherwise (n-1)*n/2 + 1.
 * https://oeis.org/A131179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n mod 2 == 0 then n*(n+1)/2 else (n-1)*n/2 + 1
 * Range:     u8 n <= 23, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1U) == 0)   /* even n */
        return ((seq_t)n * ((seq_t)n + 1)) / 2;
    else                 /* odd n, n>=1 */
        return ((((seq_t)(n - 1)) * (seq_t)n) / 2) + 1;
}
