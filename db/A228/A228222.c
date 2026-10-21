/*
 * A228222  Number of second differences of arrays of length 7 of numbers in
 *          0..n.
 * https://oeis.org/A228222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56*n^5 + 14*n^4 - 108*n^3 + 289*n^2 - 125*n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 37, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence a(n) is defined for n ≥ 1.  For n = 0 the polynomial gives 1,
       which is harmless if the function is called with 0. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;

    seq_t result = (seq_t)56 * n5
                 + (seq_t)14 * n4
                 - (seq_t)108 * n3
                 + (seq_t)289 * n2
                 - (seq_t)125 * nn
                 + (seq_t)1;

    return result;
}
