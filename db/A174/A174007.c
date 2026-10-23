/*
 * A174007  a(2n+1)=2. a(2n)= 1-n.
 * https://oeis.org/A174007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(2n+1)=2; a(2n)=1-n
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return 2 for odd indices */
    if (n & 1) {
        return (seq_t)2;
    }

    /* return 1 - n/2 for even indices */
    seq_t half = (seq_t)(n / 2);
    return (seq_t)1 - half;
}
