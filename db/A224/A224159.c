/*
 * A224159  Number of 3 X n 0..1 arrays with diagonals and rows unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A224159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8, a(2) = 36 For n > 2: a(n) = (n^6 + 3*n^5 + 235*n^4 -
 *            135*n^3 + 4444*n^2 - 2388*n + 14400) / 720 (the numerator is
 *            always divisible by 720 for n ≥ 3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 56, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)8;
    if (n == (idx_t)2)
        return (seq_t)36;

    seq_t nn = (seq_t)n;            /* n */
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */
    seq_t n4 = n3 * nn;            /* n^4 */
    seq_t n5 = n4 * nn;            /* n^5 */
    seq_t n6 = n5 * nn;            /* n^6 */

    /* Positive part of the numerator */
    seq_t pos = n6
              + (seq_t)3  * n5
              + (seq_t)235 * n4
              + (seq_t)4444 * n2
              + (seq_t)14400;

    /* Negative part of the numerator */
    seq_t neg = (seq_t)135  * n3
              + (seq_t)2388 * nn;

    seq_t numer = pos - neg;               /* (n^6 + 3n^5 + 235n^4 - 135n^3 + 4444n^2 - 2388n + 14400) */
    return numer / (seq_t)720;             /* exact division */
}
