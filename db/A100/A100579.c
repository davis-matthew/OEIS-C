/*
 * A100579  Numerator of the best rational approximation to the decimal
 *          representation of the digital roots of m^n, m=1,2,..
 * https://oeis.org/A100579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, frac, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence described by: a(n+9)=a(n) with initial terms a(1)=1, a(2)=125, a(3)=7,
   a(4)=49, a(5)=158, a(6)=17, a(7)=58, a(8)=2, a(9)=1.  These are the
   coefficients of x*A(x)/(1-x^9) where A(x)=x^8+2x^7+58x^6+17x^5+158x^4+49x^3+7x^2+125x+1. */
seq_t a(idx_t n)
{
    /* Periodic pattern of length 9 */
    static const seq_t period[9] = {1,125,7,49,158,17,58,2,1};
    return period[(n-1u)%9];
}
