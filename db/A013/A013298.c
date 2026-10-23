/*
 * A013298  cos(log(x+1)-arctanh(x))=1-3/4!*x^4-90/6!*x^6-4515/8!*x^8...
 * https://oeis.org/A013298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the integer coefficient of x^(2n) in the series
 *            cos(log(1+x) – artanh(x)). Recurrence: Let A(0)=1, B(0)=0. For
 *            k=0 … n-1: coeff = 2*k+1 A(k+1) = coeff * ( -2*k*A(k) - B(k) )
 *            B(k+1) = coeff * ( A(k) - 2*k*B(k) ) Then a(n) = (n even ? A(n)
 *            : -A(n))
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t A = 1;          /* F_k in the derivation */
    seq_t B = 0;          /* G_k in the derivation */
    for (idx_t k = 0; k < n; ++k) {
        seq_t coeff   = (seq_t)(2*k + 1);
        seq_t mul2k   = (seq_t)(2*k);

        seq_t newA = coeff * ( - (mul2k * A + B) );
        seq_t newB = coeff * (  A - mul2k * B );

        A = newA;
        B = newB;
    }
    return ( (n & 1) == 0 ) ? A : -A;
}
