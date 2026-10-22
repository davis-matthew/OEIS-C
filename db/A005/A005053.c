/*
 * A005053  Expansion of g.f. (1 - 2*x)/(1 - 5*x).
 * https://oeis.org/A005053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 3*5^(n-1) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r;
    seq_t b;
    idx_t e;

    if (n == 0) {
        return (seq_t)1;
    }
    r = (seq_t)3;
    b = (seq_t)5;
    e = n - 1;
    while (e > 0) {
        if (e & 1) {
            r *= b;
        }
        e >>= 1;
        if (e > 0) {
            b *= b;
        }
    }
    return r;
}
