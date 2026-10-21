/*
 * A212977  Number of (w,x,y) with all terms in {0,...,n} and n/2 < w+x+y <=
 *          n.
 * https://oeis.org/A212977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n^3+75*n^2+109*n-3*((n^2+7*n+11)*(-1)^n-11))/96
 *            Piecewise version for clarity and performance
 * Range:     u8 n <= 0, u16 n <= 14, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t once */
    seq_t sn = n;
    seq_t n2 = sn * sn;       /* n^2 */
    seq_t n3 = n2 * sn;       /* n^3 */
    seq_t term;

    /* branch on parity */
    if ((n & 1ULL) == 0) {    /* even n */
        /* a(n) = (14*n^3 + 72*n^2 + 88*n) / 96 */
        term = 14 * n3 + 72 * n2 + 88 * sn;
    } else {                  /* odd n */
        /* a(n) = (14*n^3 + 78*n^2 + 130*n + 66) / 96 */
        term = 14 * n3 + 78 * n2 + 130 * sn + 66;
    }

    return term / 96;
}
