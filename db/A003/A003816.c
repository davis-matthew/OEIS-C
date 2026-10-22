/*
 * A003816  a(0) = 0, a(n) = a(n-1) XOR -n.
 * https://oeis.org/A003816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4n)=0, a(4n+1)=4n+1, a(4n+2)=-1, a(4n+3)=4n+4
 * Range:     i8 n <= 67, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t remainder = n % 4;
    if (remainder == 0) {
        return 0;
    } else if (remainder == 1) {
        return (seq_t)n;
    } else if (remainder == 2) {
        return -1;
    } else {
        return (seq_t)(n + 1);
    }
}
