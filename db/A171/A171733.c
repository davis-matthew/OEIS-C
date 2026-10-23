/*
 * A171733  a(2n)=A165568(n). a(2n+1)=A165563(n).
 * https://oeis.org/A171733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^4/16 + n^3/8 + n^2/16 -15/32 + (-1)^n * ( n^3/8 +
 *            3*n^2/16 - n - 17/32 )
 * Range:     i8 n <= 2, i16 n <= 11, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;                 /* cast index to seq_t */
    seq_t n2 = ns * ns;                  /* n^2 */
    seq_t n3 = n2 * ns;                  /* n^3 */
    seq_t n4 = n3 * ns;                  /* n^4 */

    /* terms scaled by 32 to keep everything integer */
    seq_t sec = 2 * n4 + 4 * n3 + 2 * n2 - 15;          /*  n^4/16  + n^3/8 + n^2/16 -15/32  scaled */
    seq_t alt = 4 * n3 + 6 * n2 - 32 * ns - 17;        /*  4n^3 + 6n^2 -32n -17  from the alternating part */

    /* A = 32 * a(n) = sec + (-1)^n * alt */
    seq_t A = sec + ( (n & 1) ? -alt : alt );

    return A / 32;   /* divide by 32 to obtain a(n) */
}
