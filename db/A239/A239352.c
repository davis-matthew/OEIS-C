/*
 * A239352  van Heijst's upper bound on the number of squares inscribed by a
 *          real algebraic curve in R^2 of degree n, if the number is finite.
 * https://oeis.org/A239352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^2 + n - 4)/4 (OEIS A239352)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 361, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The formula is only valid for n >= 2; for 0 and 1 the value is 0. */
    if (n < 2)
        return 0;

    seq_t nn = (seq_t)n;          /* n as seq_t */
    seq_t n1 = nn - 1;            /* n-1  (safe: n>=2) */
    seq_t n2 = nn * nn;           /* n^2   */
    seq_t inner = n2 + nn;        /* n^2 + n  */
    inner -= (seq_t)4;            /* n^2 + n - 4 (inner >= 4 for n>=2) */

    seq_t prod = nn * n1;         /* n*(n-1) */
    prod = prod * inner;          /* n*(n-1)*(n^2+n-4) */

    return prod / (seq_t)4;       /* divide by 4 */
}
