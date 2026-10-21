/*
 * A076118  a(n) = Sum_{k=n/2..n} k * (-1)^(n-k) * C(k,n-k).
 * https://oeis.org/A076118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = piecewise * a(3m) = m * (-1)^m * a(3m+1) = (2m+1) *
 *            (-1)^m * a(3m+2) = (m+1) * (-1)^m * where m = floor(n/3). This
 *            is equivalent to the binomial sum in the * OEIS entry and works
 *            for all n >= 0
 * Range:     i8 n <= 192, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 3;             /* floor(n/3) */
    seq_t mm = (seq_t)m;         /* cast to seq_t for arithmetic */
    seq_t sign = ((mm & 1) ? (seq_t)(-1) : (seq_t)1); /* (-1)^m */

    switch (n % 3) {
    case 0:
        return sign * mm;                        /* a(3m)  */
    case 1:
        return sign * ((seq_t)(2 * m + 1));      /* a(3m+1) */
    case 2:
        return sign * ((seq_t)(m + 1));          /* a(3m+2) */
    default:
        return 0; /* unreachable but keeps compiler happy */
    }
}
