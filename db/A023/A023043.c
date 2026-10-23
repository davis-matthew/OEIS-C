/*
 * A023043  6th differences of factorial numbers.
 * https://oeis.org/A023043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 15*n^5 + 100*n^4 + 355*n^3 + 694*n^2 + 689*n +
 *            265) * n!
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t n4  = n3 * nn;
    seq_t n5  = n4 * nn;
    seq_t n6  = n5 * nn;

    seq_t poly = n6 + (seq_t)15 * n5
               + (seq_t)100 * n4
               + (seq_t)355 * n3
               + (seq_t)694 * n2
               + (seq_t)689 * nn
               + (seq_t)265;

    return poly * fact;
}
