/*
 * A166545  a(n) = 13*n - a(n-1), (with a(1)=13).
 * https://oeis.org/A166545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*((n+1)/2)
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS A166545: a(1) = 13, a(n) = 13*n - a(n-1). */
    /* Closed form: a(n) = 13 * ceil(n/2) = 13 * ((n+1)/2). */
    seq_t half = ((seq_t)n + 1) / 2;   /* ceil(n/2) as an integer */
    return ((seq_t)13) * half;
}
