/*
 * A022274  a(n) = n*(17*n - 1)/2.
 * https://oeis.org/A022274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(17*n - 1)/2
 * Range:     u8 n <= 3, u16 n <= 86, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use seq_t for all intermediate calculations to avoid
       overflow on lower‑bit implementations.  The division by 2
       is always exact because n*(17*n - 1) is even for every n. */
    seq_t nn = (seq_t)n;
    seq_t term = nn * ((nn * (seq_t)17) - (seq_t)1);
    return term / (seq_t)2;
}
