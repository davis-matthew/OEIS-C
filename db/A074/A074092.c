/*
 * A074092  Number of plane binary trees of size n+3 and contracted height n.
 * https://oeis.org/A074092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(n+2)*(n-1) for n ≥ 2, a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;

    seq_t pow2 = 1;                 /* compute 2^(n-1) */
    for (idx_t i = 0; i < n - 1; ++i)
        pow2 *= 2;

    seq_t factor = ((seq_t)n + 2) * ((seq_t)n - 1);
    return pow2 * factor;
}
