/*
 * A052675  Expansion of e.g.f. (1-x)/(1-5*x).
 * https://oeis.org/A052675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4, a(n) = 5*n*a(n-1) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t result = (seq_t)4;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = (seq_t)5 * (seq_t)i;
        result = result * mult;
    }
    return result;
}
