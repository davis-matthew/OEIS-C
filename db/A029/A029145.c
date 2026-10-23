/*
 * A029145  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^5)*(1-x^8)).
 * https://oeis.org/A029145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((173*n^3+63*n^2+48*n+320-9*(n+11)*(n mod 2))/288) -
 *            floor((3*n^3+n^2+2)/5)
 * Range:     u8 none, u16 n <= 7, u32 n <= 1488, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */

    /* first floor term */
    seq_t term1 = 173 * n3 + 63 * n2 + 48 * sn + 320;
    if (n & 1) {                 /* n mod 2 == 1 */
        term1 -= 9 * ((seq_t)(n + 11));
    }
    seq_t fld1 = term1 / 288;    /* floor division */

    /* second floor term */
    seq_t fld2 = (3 * n3 + n2 + 2) / 5;

    return fld1 - fld2;
}
