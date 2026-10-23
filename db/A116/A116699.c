/*
 * A116699  Number of permutations of length n which avoid the patterns 123
 *          and 4312.
 * https://oeis.org/A116699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 2*n^3 - 13*n^2 + 34*n)/24
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 401, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                  /* n */
    seq_t n2 = nn * nn;                   /* n^2 */
    seq_t n3 = n2 * nn;                   /* n^3 */
    seq_t n4 = n3 * nn;                   /* n^4 */
    seq_t num = n4 + 2*n3 - 13*n2 + 34*nn; /* numerator */
    return num / 24;                      /* integer division */
}
