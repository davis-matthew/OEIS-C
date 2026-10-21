/*
 * A228293  a(n) = Sum_{k=1..9} n^k.
 * https://oeis.org/A228293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..9} n^k (geometric series with ratio n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* first term is 0 */
    if (n == 1) return 9;           /* sum of 1 nine times */

    seq_t result = 0;
    seq_t term   = 1;
    for (idx_t i = 0; i < 9; ++i) {
        term   *= (seq_t)n;          /* term = n^{i+1} */
        result += term;             /* accumulate the sum */
    }
    return result;
}
