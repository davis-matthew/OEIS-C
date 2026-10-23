/*
 * A193260  G.f.: x+x^2 = Sum_{n>=1} x^n * ((1+x+x^2)^n - x^(2*n)) /
 *          (1+x+x^2)^a(n).
 * https://oeis.org/A193260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(log_3(n)) + \sum_{k>=1} floor(n/3^k)
 * Range:     u8 n <= 169, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1, but guard against 0 */

    /* Compute floor(log_3(n)) */
    idx_t tmp = n;
    idx_t log3 = 0;
    while (tmp >= 3) {
        tmp /= 3;
        ++log3;
    }

    /* Compute exponent of 3 in n! (sum floor(n/3^k)) */
    idx_t m = n;
    seq_t sum = 0;
    while (m > 0) {
        m /= 3;
        sum += (seq_t)m;
    }

    return (seq_t)n + (seq_t)log3 + sum;
}
