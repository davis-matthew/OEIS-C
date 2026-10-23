/*
 * A011819  M-sequences m_0,m_1,m_2,m_3 with m_1 < n.
 * https://oeis.org/A011819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( 2*(n-1)^6 +15*(n-1)^5 +50*(n-1)^4 +165*(n-1)^3 *
 *            +308*(n-1)^2 +540*(n-1) +720 ) / 360
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 95, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n=1 the argument of the polynomial is 0.  */
    idx_t m = n - 1;                 /* m = n-1  */
    unsigned __int128 M = (unsigned __int128)m;   /* 128‑bit intermediate */

    /* Evaluate the polynomial in Horner form. */
    unsigned __int128 v = M * 2 + 15;          /* 2*M + 15           */
    v = M * v + 50;                            /* M*(2*M+15)+50      */
    v = M * v + 165;                           /* M*(...) +165       */
    v = M * v + 308;                           /* M*(...) +308       */
    v = M * v + 540;                           /* M*(...) +540       */
    v = M * v + 720;                           /* M*(...) +720       */

    v /= 360;                                  /* divide by 360      */

    return (seq_t)v;                           /* cast back to seq_t */
}
