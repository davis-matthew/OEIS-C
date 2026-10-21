/*
 * A230501  Floor(Sum(d(k), k=1..n)/n), where d(k) is the number of divisors
 *          of k.
 * https://oeis.org/A230501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (1/n) * Σ_{k=1..n} d(k) ) where d(k) is the number
 *            of divisors of k
 * Range:     u8 n <= 59, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defined only for n>=1 by OEIS offset */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ) {
        idx_t q = n / i;              /* floor(n / i)  */
        idx_t last = n / q;           /* largest j with floor(n / j) == q */
        sum += (seq_t)q * (seq_t)(last - i + 1);
        i = last + 1;
    }
    return sum / (seq_t)n;             /* integer division truncates toward zero, same as floor for positive */
}
