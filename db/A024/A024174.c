/*
 * A024174  a(n) is floor((4th elementary symmetric function of 1,2,..,n)/(3rd
 *          elementary symmetric function of 1,2,...,n)).
 * https://oeis.org/A024174
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^2 - 3*n - 2)/8) for n >= 4; a(3) = 0; offset 3
 * Range:     u8 n <= 17, u16 n <= 300, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                 /* a(3) = 0 and for n = 4 the formula gives 0 */
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t num = nn - 3 * (seq_t)n - 2;   /* numerator is non‑negative for n >= 4 */
    return num / 8;                          /* integer division = floor */
}
