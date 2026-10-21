/*
 * A224460  The hyper-Wiener index of the straight pentachain of n pentagonal
 *          rings (see Fig. 2.1 in the A. A. Ali et al. reference).
 * https://oeis.org/A224460
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4 +34*n^3 +145*n^2 -190*n +208)/8
 * Range:     u8 none, u16 n <= 9, u32 n <= 323, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* behaviour below the offset is undefined */

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;             /* n^2 */
    seq_t n3 = n2 * nn;             /* n^3 */
    seq_t n4 = n3 * nn;             /* n^4 */

    seq_t num = 3 * n4
              + 34 * n3
              + 145 * n2
              - 190 * nn
              + 208;                /* numerator */

    return num >> 3;                /* divide by 8 (numerator always divisible by 8) */
}
