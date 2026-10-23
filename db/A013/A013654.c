/*
 * A013654  Positive nonsquare integers k such that each term of the regular
 *          continued fraction of sqrt(k) divides k.
 * https://oeis.org/A013654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1) if n is odd, a(n) = n*(n+2) if n is even
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (nn & 1) {          /* odd n */
        return nn * (nn + 1);
    } else {               /* even n */
        return nn * (nn + 2);
    }
}
