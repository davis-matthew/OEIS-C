/*
 * A211617  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and 2w+x+y>0.
 * https://oeis.org/A211617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 - n^2 + n if n is even * a(n) = 4*n^3 - n^2 + n - 1
 *            if n is odd * (OEIS A211617)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */
    seq_t res = 4 * n3 - n2 + sn;
    if ((n & 1) == 1)            /* n odd */
        res -= 1;
    return res;
}
