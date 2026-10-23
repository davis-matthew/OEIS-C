/*
 * A019425  Continued fraction for tan(1/2).
 * https://oeis.org/A019425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0) = 0; a(1) = 1; a(2) = 1; for n>=3: a(n) = (n is even) ? 1 :
 *            2*(n-1)
 * Range:     u8 n <= 128, u16 n <= 9567, u32 n <= 9567, u64 n <= 9567,
 *            u128 n <= 9567, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) */
    if ((n & 1ULL) == 0) return 1;      /* even indices (n>=2) */
    if (n == 1) return 1;               /* a(1) */
    return 2 * ((seq_t)n - 1);          /* odd indices n>=3  */
}
