/*
 * A086024  a(n) = Sum_{i=1..n} C(i+3,4)^3.
 * https://oeis.org/A086024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} C(i+3,4)^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 74, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* compute C(i+3,4) = (i*(i+1)*(i+2)*(i+3))/24  */
        unsigned long long num = (unsigned long long)i * (i + 1) * (i + 2) * (i + 3);
        unsigned long long c_ll = num / 24;              /* 24 = 4! */
        seq_t c = (seq_t)c_ll;                           /* cast to seq_t */
        seq_t term = c * c * c;                          /* cube */
        sum += term;
    }
    return sum;
}
