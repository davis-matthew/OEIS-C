/*
 * A093610  Lower Beatty sequence for e^G, G = Euler's gamma constant.
 * https://oeis.org/A093610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(k+1)/k) , k = e^G ≈
 *            1.7810724179901979852368632679039
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = floor(n * alpha) where alpha = (k+1)/k ≈ 1.561459483990197985 */
seq_t a(idx_t n)
{
    /* alpha is approximated by the rational num/den (error < 1e-15) */
    static const unsigned __int128 ALPHA_NUM = 1561459483990197ULL;
    static const unsigned __int128 ALPHA_DEN = 1000000000000000ULL;

    if (n == 0) return 0;  /* under offset, value irrelevant */
    unsigned __int128 prod = (unsigned __int128)n * ALPHA_NUM;
    unsigned __int128 val  = prod / ALPHA_DEN;
    return (seq_t)val;
}
