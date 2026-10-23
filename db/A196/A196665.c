/*
 * A196665  Expansion of g.f. (1-6*x)/(1-19*x).
 * https://oeis.org/A196665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A196665
   Formula:  a(0) = 1, a(n) = 13*19^(n-1)   for n > 0
   (equivalently: a(1) = 13; a(n) = 19*a(n-1) for n > 1) */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t result = (seq_t)13;
    for (idx_t i = (idx_t)1; i < n; ++i)
        result *= (seq_t)19;

    return result;
}
