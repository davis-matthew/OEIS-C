/*
 * A131358  a(3*k) = 0, a(3*k+1) = k+1, a(3*k+2) = -k.
 * https://oeis.org/A131358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(3k)=0; a(3k+1)=k+1; a(3k+2)=-k. Offset 0
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r   = n % 3;
    seq_t k   = (seq_t)(n / 3);
    if (r == 0) {
        return (seq_t)0;
    } else if (r == 1) {
        return k + 1;
    } else { /* r == 2 */
        return -k;
    }
}
