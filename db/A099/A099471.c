/*
 * A099471  A sequence generated from the Quadrifoil (flat knot).
 * https://oeis.org/A099471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(3k) = (-1)^k * (2k+1) a(3k+1) = (-1)^k * k a(3k+2) =
 *            (-1)^(k+1) * (k+2)
 * Range:     i8 n <= 191, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    unsigned r = n % 3;
    seq_t sign1 = (k & 1) ? (seq_t)-1 : (seq_t)1;           // (-1)^k
    seq_t sign2 = ((k + 1) & 1) ? (seq_t)-1 : (seq_t)1;     // (-1)^(k+1)

    switch (r) {
    case 0: // a(3k)
        return sign1 * (seq_t)(2 * k + 1);
    case 1: // a(3k+1)
        return sign1 * (seq_t)k;
    case 2: // a(3k+2)
        return sign2 * (seq_t)(k + 2);
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
