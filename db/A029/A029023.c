/*
 * A029023  Expansion of 1/((1-x)*(1-x^2)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A029023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 20)*(2*n^2 + 23*n + 114)/1848)
 * Range:     u8 n <= 51, u16 n <= 382, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                       /* Promote index to seq_t */
    unsigned __int128 x = (unsigned __int128)nn;

    /* Compute numerator in 128‑bit integer to avoid overflow for
       typical n up to at least several millions */
    unsigned __int128 numerator = (x + 20u) *
        ((unsigned __int128)2 * x * x + (unsigned __int128)23 * x + 114u);

    /* Final division by 1848, truncates toward zero as required */
    return (seq_t)(numerator / 1848u);
}
