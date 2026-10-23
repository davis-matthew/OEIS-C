/*
 * A133655  a(n) = 2*A016777(n) + A016777(n-1) - (n+1).
 * https://oeis.org/A133655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A016777(n) + A016777(n-1) - (n+1)
 * Range:     u8 n <= 32, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplification: A016777(n) = 3*n + 1  →  a(n) = 8*n - 1 for n ≥ 1, a(0)=1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return (seq_t)n * ((seq_t)8) - (seq_t)1;
}
