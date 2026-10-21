/*
 * A221013  The hyper-Wiener index of the nanostar dendrimer G_n , defined
 *          pictorially in the Golriz - Darafsheh - Khalifeh reference.
 * https://oeis.org/A221013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27060 + 2^n*(-4550*n^2 +16990*n -243062) +
 *            4^n*(67600*n^2-133640*n+219724)
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;

    /* Compute 2^n and 4^n iteratively */
    seq_t two_n  = 1;
    seq_t four_n = 1;
    for (idx_t i = 0; i < n; ++i) {
        two_n  *= (seq_t)2;
        four_n *= (seq_t)4;
    }

    /* Positive part of the 2^n coefficient: 4550*n^2 - 16990*n + 243062 */
    seq_t poly2pos = (seq_t)4550 * N2 - (seq_t)16990 * N + (seq_t)243062;

    /* 4^n coefficient: 67600*n^2 - 133640*n + 219724 */
    seq_t poly4 = (seq_t)67600 * N2 - (seq_t)133640 * N + (seq_t)219724;

    /* Assemble the result: 27060 + four_n*poly4 - two_n*poly2pos */
    seq_t result = (seq_t)27060 + four_n * poly4;
    result -= two_n * poly2pos;

    return result;
}
