/*
 * A233329  Expansion of (1+4*x+x^2)/((1+x)^2*(1-x)^5).
 * https://oeis.org/A233329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (2*(n^4+10*n^3+34*n^2+(45+(-1)^(n+1))*n)+37+5*(-1)^(n+1))/32
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work with seq_t to avoid overflow on all supported builds. */
    seq_t s   = (seq_t)n;          /* n as seq_t */
    seq_t n2  = s * s;             /* n^2 */
    seq_t n3  = n2 * s;            /* n^3 */
    seq_t n4  = n3 * s;            /* n^4 */

    seq_t poly;
    seq_t tmp;
    seq_t result;

    if ((n & 1) == 0) {            /* n even : (-1)^(n+1) = -1 */
        poly = n4 + (seq_t)10 * n3 + (seq_t)34 * n2 + (seq_t)44 * s;
        tmp  = (seq_t)2 * poly + (seq_t)32;  /* 37 + 5*(-1) = 32 */
        result = tmp / (seq_t)32;
    } else {                       /* n odd  : (-1)^(n+1) = +1 */
        poly = n4 + (seq_t)10 * n3 + (seq_t)34 * n2 + (seq_t)46 * s;
        tmp  = (seq_t)2 * poly + (seq_t)42;  /* 37 + 5*(+1) = 42 */
        result = tmp / (seq_t)32;
    }

    return result;
}
