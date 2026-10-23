/*
 * A015996  (tau(n^4) + 3)/4, where tau = A000005.
 * https://oeis.org/A015996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n^4)+3)/4
 * Range:     u8 n <= 419, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offsets below 1 */

    idx_t tmp = n;
    seq_t prod = 1;                /* product of (4*e_i + 1) */

    for (idx_t i = 2; i <= tmp / i; ++i) {
        if (tmp % i == 0) {
            idx_t e = 0;
            while (tmp % i == 0) {
                ++e;
                tmp /= i;
            }
            prod *= (seq_t)(4 * e + 1);
        }
    }

    if (tmp > 1) {                 /* leftover prime factor with exponent 1 */
        prod *= (seq_t)5;          /* 4*1+1 = 5 */
    }

    return (prod + (seq_t)3) / (seq_t)4;
}
