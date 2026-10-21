/*
 * A085439  a(n) = Sum_{i=1..n} binomial(i+1,2)^4.
 * https://oeis.org/A085439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} ((i+1 choose 2)^4)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 239, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t sum = 0;
    seq_t t   = 0;               /* will hold i*(i+1)/2 */
    for (idx_t i = 1; i <= n; ++i) {
        t += (seq_t)i;           /* t = i*(i+1)/2 via recurrence */
        seq_t t2 = t * t;        /* (i*(i+1)/2)^2    */
        seq_t t4 = t2 * t2;      /* (i*(i+1)/2)^4    */
        sum += t4;               /* accumulate the sum */
    }
    return sum;
}
