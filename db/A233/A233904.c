/*
 * A233904  a(2n) = a(n) - n, a(2n+1) = a(n) + n, with a(0)=0.
 * https://oeis.org/A233904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0; a(2n) = a(n) - n; a(2n+1) = a(n) + n;
 * Range:     i8 n <= 143, i16 n <= 8192, i32 n <= 8192, i64 n <= 8192,
 *            i128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    idx_t q = n >> 1;            /* n = 2*q + (n&1) */
    seq_t val = a(q);            /* recursively compute a(q) */
    if ((n & 1) == 0) {          /* even case */
        return val - (seq_t)q;
    } else {                     /* odd case */
        return val + (seq_t)q;
    }
}
