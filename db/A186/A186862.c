/*
 * A186862  Number of 3-step king's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A186862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(n-1)*(7*n-11)
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    return (seq_t)8 * (t - (seq_t)1) * ((seq_t)7 * t - (seq_t)11);
}
