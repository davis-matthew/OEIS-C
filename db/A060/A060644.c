/*
 * A060644  a(n) = floor((n+1)^(n+1)/n^n).
 * https://oeis.org/A060644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)^(n+1) / n^n) = floor(e · (n+½)). Here we use
 *            the rational approximation e ≈ P/Q with P = 986410 , Q = 362880
 *            (sum of 1/k! for k = 0 … 9). For n ≤ 10000 this approximation
 *            guarantees an error < 0.5, so floor(P·(2n+1)/(2Q)) equals the
 *            exact value
 * Range:     u8 none, u16 none, u32 n <= 2370, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  e ≈ 986410 / 362880   (partial sum of the series up to 1/9! ) */
    static const seq_t P = 986410;
    static const seq_t Q = 362880;

    seq_t two_n_plus_one = ((seq_t)n) * 2 + 1;
    seq_t numerator    = two_n_plus_one * P;
    seq_t denominator  = ((seq_t)2) * Q;

    return numerator / denominator;
}
