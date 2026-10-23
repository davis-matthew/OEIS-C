/*
 * A194800  Number of grid points that are covered after n-th stage of
 *          A139250, assuming the vertical toothpicks have length 2 and the
 *          horizontal toothpicks have length 4.
 * https://oeis.org/A194800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=0, a(n)=n^2+3n-1 if n is odd, * a(n)=2n^2-2n+7 if n is even
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {           /* even n */
        return 2 * sn * sn - 2 * sn + 7;
    } else {                      /* odd n */
        return sn * sn + 3 * sn - 1;
    }
}
