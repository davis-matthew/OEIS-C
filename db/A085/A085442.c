/*
 * A085442  a(n) = Sum_{i=1..n} binomial(i+1,2)^7.
 * https://oeis.org/A085442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} binomial(i+1,2)^7
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 30, u128 n <= 612,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t sum = 0;
    seq_t tri = 0; /* triangular number T_{i-1} */
    for (idx_t i = 1; i <= n; ++i) {
        tri += (seq_t)i;               /* T_i = T_{i-1} + i */
        seq_t t2 = tri * tri;          /* T^2 */
        seq_t t4 = t2 * t2;            /* T^4 */
        seq_t t6 = t4 * t2;            /* T^6 */
        seq_t t7 = t6 * tri;           /* T^7 */
        sum += t7;
    }
    return sum;
}
