/*
 * A173248  a(0) = -1, otherwise a(n) = (-1)^n*(n^3 - 15*n^2 + 2*n - 12)/6.
 * https://oeis.org/A173248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1; a(n) = (-1)^n*(n^3 - 15*n^2 + 2*n - 12)/6 for n >= 1
 * Range:     i8 n <= 3, i16 n <= 37, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0u)
        return (seq_t)-1;

    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;            /* n^2   : 0 ≤ n2 ≤ n^2 */
    seq_t num  = nn * n2              /* n^3   */
                - (seq_t)15 * n2      /* -15*n^2 */
                + (seq_t)2 * nn       /* +2*n   */
                - (seq_t)12;          /* -12    */
    seq_t val   = num / (seq_t)6;     /* exact division for all n ≥ 0 */
    if (n & 1u)
        val = -val;                   /* (-1)^n * value */

    return val;
}
