/*
 * A029000  Expansion of 1/((1-x)*(1-x^2)*(1-x^3)*(1-x^6)).
 * https://oeis.org/A029000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor((2*n^3 + 36*n^2 + 183*n + 24*n*[(n mod 3)=0] +
 *            9*n*(-1)^n)/432)
 * Range:     u8 n <= 0, u16 n <= 26, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use seq_t arithmetic to avoid overflows. */
    seq_t nn  = (seq_t)n;         /* n */
    seq_t n2  = nn * nn;          /* n^2 */
    seq_t n3  = n2 * nn;          /* n^3 */

    seq_t poly = 0;

    /* 2*n^3 */
    poly += (seq_t)2 * n3;
    /* 36*n^2 */
    poly += (seq_t)36 * n2;
    /* 183*n */
    poly += (seq_t)183 * nn;

    /* 24*n when n mod 3 = 0 */
    if ((n % 3) == 0)
        poly += (seq_t)24 * nn;

    /* 9*n*(-1)^n : add if n even, subtract if n odd */
    seq_t term5 = (seq_t)9 * nn;
    if ((n % 2) == 0)
        poly += term5;   /* +9*n for even n */
    else
        poly -= term5;   /* -9*n for odd n */

    /* integer division (poly is non‑negative) */
    seq_t q   = poly / (seq_t)432;
    seq_t res = (seq_t)1 + q;

    return res;
}
