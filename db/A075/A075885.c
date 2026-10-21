/*
 * A075885  a(n) = 1 + n + n*[n/2] + n*[n/2]*[n/3] + n*[n/2]*[n/3]*[n/4] +...
 *          where [x]=floor(x).
 * https://oeis.org/A075885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + Sum_{m=1..n} Product_{k=1..m} floor(n/k)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = 1;   /* initial term a(0) */
    seq_t prod = 1;   /* running product for current m */

    for (idx_t m = 1; m <= n; ++m) {
        prod *= (seq_t)(n / m); /* floor(n/m) */
        sum += prod;
    }
    return sum;
}
