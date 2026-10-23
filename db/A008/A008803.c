/*
 * A008803  Molien series for group [2,10]+ = 2 2 10.
 * https://oeis.org/A008803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((11*(2*n+3)*(-1)^n + 2*n^2 + 6*n + 79)/80)
 * Range:     u8 n <= 3, u16 n <= 236, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_r       = (seq_t)n;
    seq_t two_n     = (seq_t)2 * n_r;
    seq_t twon_plus3 = two_n + (seq_t)3;
    seq_t term1     = (seq_t)11 * twon_plus3;              // 11*(2n+3)
    seq_t term2     = (seq_t)2 * n_r * n_r;                // 2*n^2
    seq_t term3     = (seq_t)6 * n_r;                      // 6*n
    seq_t sum_other = term2 + term3 + (seq_t)79;           // 2*n^2 + 6*n + 79

    seq_t numerator;
    if ((n & 1) == 0) {          /* even n: add the alternating term */
        numerator = term1 + sum_other;
    } else {                     /* odd n: subtract the alternating term */
        numerator = sum_other - term1;
    }

    return numerator / (seq_t)80; /* integer division gives the floor */
}
