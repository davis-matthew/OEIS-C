/*
 * A024312  a(n) = s(1)*s(n) + s(2)*s(n-1) + ... + s(k)*s(n+1-k), where k =
 *          floor((n+1)/2), s = (natural numbers >= 3).
 * https://oeis.org/A024312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((75 + 182*n + 63*n^2 + 4*n^3) - 3*(25 + 10*n +
 *            n^2)*(-1)^n)/48
 * Range:     u8 none, u16 n <= 20, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 (OEIS offset 1). */
    seq_t nn  = (seq_t)n;          /* cast to seq_t to avoid overflow in intermediate */
    seq_t n2  = nn * nn;           /* n^2 */
    seq_t n3  = n2 * nn;           /* n^3 */
    seq_t s   = (seq_t)25 + (seq_t)10 * nn + n2; /* 25 + 10*n + n^2 */

    /* term1 = 75 + 182*n + 63*n^2 + 4*n^3 */
    seq_t term1 = (seq_t)75
                + (seq_t)182 * nn
                + (seq_t)63 * n2
                + (seq_t)4 * n3;

    /* 3 * s */
    seq_t three_s = (seq_t)3 * s;

    /* numerator: if n is even subtract, if odd add the 3*s term */
    seq_t numer = ((n & 1ULL) == 0) ? (term1 - three_s) : (term1 + three_s);

    /* division by 48 (exact) */
    return numer / 48;
}
