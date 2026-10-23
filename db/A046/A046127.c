/*
 * A046127  a(0) = 0; for n>0, a(n) = maximal number of regions into which
 *          space can be divided by n spheres.
 * https://oeis.org/A046127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n^2 - 3*n + 8)/3
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nn   = (seq_t)n;
    seq_t tmp  = nn * nn - 3 * nn + 8;   /* n^2 - 3n + 8 */
    seq_t val  = nn * tmp;              /* n*(n^2 - 3n + 8) */
    return val / (seq_t)3;              /* division is exact */
}
