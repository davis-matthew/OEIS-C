/*
 * A168388  First number in the n-th row of A172002.
 * https://oeis.org/A168388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12 + n + 3*(-1)^n*n + 2*n^3) / 12
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent piecewise:                                 */
/*   if n even -> (2*n^3 + 4*n + 12) / 12                */
/*   if n odd  -> (2*n^3 - 2*n + 12) / 12                */

seq_t a(idx_t n)
{
    /* Promote idx_t to seq_t for arithmetic */
    seq_t fn   = (seq_t)n;
    seq_t n2   = fn * fn;
    seq_t n3   = n2 * fn;

    seq_t numerator;
    if ((n & 1) == 0) {
        /* n is even: 2*n^3 + 4*n + 12 */
        numerator = ((seq_t)2 * n3) + ((seq_t)4 * fn) + (seq_t)12;
    } else {
        /* n is odd:  2*n^3 - 2*n + 12 */
        numerator = ((seq_t)2 * n3) - ((seq_t)2 * fn) + (seq_t)12;
    }

    /* Result is always divisible by 12 */
    return numerator / (seq_t)12;
}
