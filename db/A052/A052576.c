/*
 * A052576  E.g.f. (1+x^2-2x^3)/(1-2x).
 * https://oeis.org/A052576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n * n! for n ≥ 3; special cases: a(0) = 1, a(1) = 2,
 *            a(2) = 10
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small special cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)10;

    /* For n ≥ 3 compute 2^n * n! iteratively. */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        res *= (seq_t)i;   /* multiply by i! */
        res *= (seq_t)2;   /* multiply by 2 each time */
    }
    return res;
}
