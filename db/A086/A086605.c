/*
 * A086605  a(n) = 9*n^3 - 18*n^2 + 10*n.
 * https://oeis.org/A086605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*n^3 - 18*n^2 + 10*n
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 782, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t2 = tn * tn;
    seq_t t3 = t2 * tn;
    seq_t res = (seq_t)9 * t3 - (seq_t)18 * t2 + (seq_t)10 * tn;
    return res;
}
