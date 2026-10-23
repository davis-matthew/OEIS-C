/*
 * A173035  Cat years in human years: a(0) = 0, a(1) = 15, a(2) = 24, a(n) =
 *          a(n-1) + 4 for n >= 3.
 * https://oeis.org/A173035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n=0; 15 if n=1; 24 + 4*(n-2) for n>=2
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 15;
    return ((seq_t)24) + (((seq_t)n - 2) * ((seq_t)4));
}
