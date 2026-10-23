/*
 * A166445  Hankel transform of A025276.
 * https://oeis.org/A166445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(4k)=2k+1, a(4k+1)=0, a(4k+2)=-(2k+1), a(4k+3)=1
 * Range:     i8 n <= 255, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)(n >> 2);   /* n / 4 */
    unsigned r = n & 3;          /* n % 4 */
    switch (r) {
    case 0: return 2*k + 1;     /* a(4k)   */
    case 1: return 0;           /* a(4k+1) */
    case 2: return -(2*k + 1); /* a(4k+2) */
    case 3: return 1;           /* a(4k+3) */
    }
    return 0; /* unreachable */
}
