/*
 * A201347  Number of n X 2 0..1 arrays with rows and columns
 *          lexicographically nondecreasing and every element equal to at
 *          least one horizontal or vertical neighbor.
 * https://oeis.org/A201347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 - 3 n^2 + 20 n - 12)/6 for n>1 ; a(1) = 2
 *            (equivalent to a(n) = 4*a(n-1)-6*a(n-2)+4*a(n-3)-a(n-4) for n>5)
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the only exceptional case */
    if (n == 1)
        return 2;

    /* n>=2: use the closed‑form integer expression */
    seq_t nn = (seq_t)n;

    /* Compute the numerator safely in unsigned arithmetic
       term = nn^3 + 20*nn - (3*nn^2 + 12)   */
    seq_t part1 = nn * nn * nn + 20 * nn;
    seq_t part2 = 3 * nn * nn + 12;
    seq_t term  = part1 - part2;

    /* The division by 6 is exact for all n>1 */
    return term / 6;
}
