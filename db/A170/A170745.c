/*
 * A170745  Expansion of g.f.: (1+x)/(1-25*x).
 * https://oeis.org/A170745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A170745: a(0) = 1; a(n) = 26*25^(n-1) for n > 0. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t pow25 = 1;
    for (idx_t i = 1; i < n; ++i)
    {
        pow25 *= (seq_t)25;
    }
    return (seq_t)26 * pow25;
}
