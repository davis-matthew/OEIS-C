/*
 * A175346  a(n) = Sum_{k=1..n^2} d(k), d(k) = number of divisors of k
 *          (A000005).
 * https://oeis.org/A175346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=1}^{n^2} d(k) which equals \sum_{i=1}^{n^2}
 *            floor(n^2 / i)
 * Range:     u8 n <= 7, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, but return 0 safely      */

    /* N = n^2; use seq_t to avoid overflow if seq_t is larger than 64 bits  */
    seq_t N = (seq_t)n * (seq_t)n;

    seq_t sum = 0;
    seq_t i = 1;
    while (i <= N) {
        seq_t q = N / i;                /* floor(N/i)  */
        seq_t next = N / q;             /* largest j with floor(N/j)=q  */
        sum += (next - i + 1) * q;      /* add contribution of all i..next */
        i = next + 1;
    }
    return sum;
}
