/*
 * A140148  a(1)=1, a(n)=a(n-1)+n^2 if n odd, a(n)=a(n-1)+ n^0 if n is even.
 * https://oeis.org/A140148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 3*n^2 + 7*n - 3 + (-3*n^2 - 3*n + 3)*(-1)^n) /
 *            12
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 (offset is 1) */
    if (n == 0) return 0;   /* defined only for n >= 1, guard anyway */

    /* Convert index to seq_t so that all arithmetic is done with seq_t */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;     /* n^2 */
    seq_t n3 = n2 * nn;     /* n^3 */

    /* Compute the two parts of the closed formula */
    seq_t sum1 = (seq_t)2 * n3 + (seq_t)3 * n2 + (seq_t)7 * nn - (seq_t)3;
    seq_t m    = (seq_t)3 * n2 + (seq_t)3 * nn - (seq_t)3;

    /* Even n: add (-1)*m  (i.e., subtract m), odd n: add m */
    seq_t term;
    if ((n & 1ULL) == 0) {      /* even */
        term = sum1 - m;
    } else {                    /* odd */
        term = sum1 + m;
    }

    /* Final division by 12 */
    return term / (seq_t)12;
}
