/*
 * A008676  Expansion of 1/((1-x^3)*(1-x^5)).
 * https://oeis.org/A008676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n+5)/5) - floor((n+2)/3)
 * Range:     u8 n <= 281, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* directly type‑cast the index to the sequence type before doing
       arithmetic – this avoids any unintended implicit conversions.   */
    seq_t N = (seq_t)n;

    /* compute the two floor divisions as integer divisions in seq_t   */
    seq_t first  = (N * 2 + (seq_t)5) / (seq_t)5;
    seq_t second = (N + (seq_t)2) / (seq_t)3;

    return first - second;
}
