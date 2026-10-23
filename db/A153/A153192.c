/*
 * A153192  Numbers such that the numerator of floor(sqrt(n))/n, when reduced
 *          to its lowest terms, is equal to 2.
 * https://oeis.org/A153192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+3)*(2*n-(-1)^n+1)/4
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but return 0 for safety */

    seq_t sn   = (seq_t)n;
    seq_t twon = sn + sn;          /* 2 * n */
    seq_t t1   = twon + 3;         /* 2*n + 3 */
    seq_t t2;

    /* For n even: (-1)^n = 1 → t2 = 2*n
       For n odd:  (-1)^n = -1 → t2 = 2*n + 2  */
    if ((n & 1) == 0) {
        t2 = twon;
    } else {
        t2 = twon + 2;
    }

    return (t1 * t2) / 4;
}
