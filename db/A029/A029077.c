/*
 * A029077  Expansion of 1/((1-x)*(1-x^4)*(1-x^7)*(1-x^12)).
 * https://oeis.org/A029077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3 + 36*n^2 + 369*n + 2016)/2016 +
 *            ((n+5)/96)*(3*[n≡0 (mod 4)] - [n≡2 (mod 4)]))
 * Range:     u8 none, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* base term A = n^3 + 36*n^2 + 369*n + 2016 */
    seq_t A = n3 + 36 * n2 + 369 * nn + 2016;

    /* additional term 21*(n+5) will be added or subtracted
       (21 = 2016 / 96) */
    seq_t add_term = 21 * (nn + 5);

    /* determine coefficient depending on n mod 4 */
    idx_t r = n % 4;
    seq_t T;
    if (r == 0) {
        /* coeff = 3 */
        T = A + 3 * add_term;
    } else if (r == 2) {
        /* coeff = -1 */
        T = A - add_term;
    } else {
        /* coeff = 0 */
        T = A;
    }

    return T / 2016;
}
