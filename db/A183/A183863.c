/*
 * A183863  a(n) = n-1+ceiling((2/5)*(-1/2+n^2)); complement of A183862.
 * https://oeis.org/A183863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1+ceiling((2/5)*(-1/2 + n^2))
 * Range:     u8 n <= 15, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* values for n<1 are unspecified */
    seq_t nn = (seq_t)n * (seq_t)n;      /* n² */
    seq_t term = (nn + nn + 3) / 5;      /* ceil((2*n²-1)/5) == (2*n²+3)/5 */
    return term + (seq_t)n - 1;          /* add n-1 */
}
