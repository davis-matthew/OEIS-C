/*
 * A053307  Number of nonnegative integer 2 X 2 matrices with sum of elements
 *          equal to n, under row and column permutations.
 * https://oeis.org/A053307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+2)*(2*n^2 + 8*n + 15 + 9*(-1)^n)/48
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 1801, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;                     /* n^2 */
    seq_t sum1 = 2 * nn2 + 8 * nn + 15;      /* 2*n^2 + 8*n + 15 */

    /* add or subtract the 9*(-1)^n term */
    if ((n & 1) == 0)
        sum1 += 9;            /* even n : +9 */
    else
        sum1 -= 9;            /* odd  n : -9 */

    seq_t result = (nn + 2) * sum1;          /* (n+2)*sum1 */
    result /= 48;                            /* divide by 48, exact integer division */

    return result;
}
