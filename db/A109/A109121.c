/*
 * A109121  a(n) = 10(n+1)^3*(2n+1)(7n+5)^2.
 * https://oeis.org/A109121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10 * (n+1)^3 * (2n+1) * (7n+5)^2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;                  /* convert index to seq_t */
    seq_t np1 = n_s + 1;                   /* (n+1) */
    seq_t np1_cub = np1 * np1 * np1;       /* (n+1)^3 */
    seq_t two_n_plus1 = (seq_t)2 * n_s + 1; /* (2n+1) */
    seq_t seven_n_plus5 = (seq_t)7 * n_s + 5; /* (7n+5) */
    seq_t seven_n_plus5_sq = seven_n_plus5 * seven_n_plus5; /* (7n+5)^2 */

    seq_t result = (seq_t)10 * np1_cub * two_n_plus1 * seven_n_plus5_sq;
    return result;
}
