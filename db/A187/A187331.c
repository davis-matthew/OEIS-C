/*
 * A187331  a(n) = Sum_{k=1..4} floor(k*n/4).
 * https://oeis.org/A187331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/4⌋ + ⌊n/2⌋ + ⌊3n/4⌋ (sum_{k=1..4} ⌊k*n/4⌋)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn          = (seq_t)n;           /* convert index to seq_t */
    seq_t floor_n_4   = nn / 4;             /* ⌊n/4⌋                */
    seq_t floor_n_2   = nn / 2;             /* ⌊n/2⌋                */
    seq_t ceil_n_4    = floor_n_4 + ((nn % 4) ? (seq_t)1 : (seq_t)0);
                                            /* ⌈n/4⌉                  */
    seq_t floor_3n_4  = nn - ceil_n_4;       /* ⌊3n/4⌋ = n – ⌈n/4⌉      */

    return nn + floor_n_4 + floor_n_2 + floor_3n_4;
}
