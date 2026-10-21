/*
 * A212971  Number of triples (w,x,y) with all terms in {0,...,n} and w <
 *          floor((x+y)/3).
 * https://oeis.org/A212971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n^2 - n - 1 + (((n+1) mod 3) mod 2))/3 * a(0) = 0,
 *            a(1) = 0, a(2) = 3, a(3) = 11, ... (OEIS sequence A212971)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t numerator = tn * tn * tn + tn * tn - tn - (seq_t)1;
    idx_t mod3 = (n + 1) % 3;
    seq_t mod2 = (seq_t)(mod3 % 2);
    numerator += mod2;
    return numerator / (seq_t)3;
}
