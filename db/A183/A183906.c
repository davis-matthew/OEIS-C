/*
 * A183906  Number of nondecreasing arrangements of n+2 numbers in 0..3 with
 *          each number being the sum mod 4 of two others.
 * https://oeis.org/A183906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 5*n)/6 + 2*n^2 - 8 for n > 2 * a(1) = 2, a(2) = 5
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 199, u64 n <= 199,
 *            u128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 5;

    seq_t nn = (seq_t)n;             /* cast index to seq_t */
    seq_t temp = nn * nn * nn + 5 * nn;   /* n^3 + 5*n */
    temp = temp / 6;                  /* divide by 6 */
    return temp + 2 * nn * nn - 8;     /* add 2*n^2 and subtract 8 */
}
