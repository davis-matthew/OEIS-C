/*
 * A176447  a(2n) = -n, a(2n+1) = 2n+1.
 * https://oeis.org/A176447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult, changed
 * Formula:   a(2n) = -n, a(2n+1) = 2n+1
 * Range:     i8 n <= 128, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is even return -(n/2), else return n.  All arithmetic is
     * performed in seq_t, the return type, while the index n is an
     * idx_t.  Division by 2 is safe once it is known that n is even.   */
    if ((n & 1) == 0)
        return -(seq_t)(n / 2);
    else
        return (seq_t)n;
}
