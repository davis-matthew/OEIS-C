/*
 * A184537  a(n) = floor(1/{(2+n^4)^(1/4)}), where {} = fractional part.
 * https://oeis.org/A184537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=5, a(1)=3, a(n)=2*n^3 for n≥2, derived from the given *
 *            recurrence and initial terms
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 5;
    if (n == 1)
        return 3;
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn * nn;
    res *= 2;
    return res;
}
