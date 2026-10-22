/*
 * A000989  3-adic valuation of binomial(2*n, n): largest k such that 3^k
 *          divides binomial(2*n, n).
 * https://oeis.org/A000989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k>=0} floor(2*n/3^k) - 2*Sum_{k>=0} floor(n/3^k)
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum1 = 0, sum2 = 0;
    idx_t power_of_3 = 1;
    
    while (power_of_3 <= 2 * n) {
        sum1 += (seq_t)(2 * n / power_of_3);
        sum2 += (seq_t)(n / power_of_3);
        if (power_of_3 > (idx_t)-1 / 3) break;
        power_of_3 *= 3;
    }
    
    return sum1 - 2 * sum2;
}
