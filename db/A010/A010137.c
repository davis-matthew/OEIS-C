/*
 * A010137  Continued fraction for sqrt(47).
 * https://oeis.org/A010137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=6; for n>=1, a(n) repeats the cycle 1,5,1,12 (period 4)
 * Range:     u8 n <= 9162, u16 n <= 9162, u32 n <= 9162, u64 n <= 9162,
 *            u128 n <= 9162, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS entry A010137 for all n >= 0. */
seq_t a(idx_t n)
{
    /* The first term is special. */
    if (n == 0)
        return (seq_t)6;

    /* Periodic cycle after the first term: 1,5,1,12. */
    static const seq_t cycle[4] = { (seq_t)1, (seq_t)5, (seq_t)1, (seq_t)12 };

    /* (n-1) % 4 selects the element of the cycle. */
    idx_t idx = (n - 1) % 4;
    return cycle[idx];
}
