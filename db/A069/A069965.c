/*
 * A069965  (Sum of digits of n)^5 - (sum of digits^5 of n).
 * https://oeis.org/A069965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (sum of digits of n)^5 - sum of (each digit)^5
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* digit sum and digit^5 sum are zero */

    idx_t tmp = n;
    unsigned long long digit_sum = 0;     /* 0 ≤ digit_sum ≤ 180 for 64-bit n */
    unsigned long long digit5_sum = 0;    /* ≤ 1.2×10^6 */

    /* extract decimal digits */
    while (tmp > 0) {
        unsigned d = tmp % 10;
        digit_sum += d;
        unsigned long long d5 = d;       /* d^1 */
        d5 *= d;                         /* d^2 */
        d5 *= d;                         /* d^3 */
        d5 *= d;                         /* d^4 */
        d5 *= d;                         /* d^5 */
        digit5_sum += d5;
        tmp /= 10;
    }

    /* compute (sum)^5 */
    seq_t s = (seq_t)digit_sum;
    seq_t s_pow5 = s * s * s * s * s;    /* safe, value ≤ 1.9e11 */

    seq_t d5_sum = (seq_t)digit5_sum;
    seq_t result = s_pow5 - d5_sum;
    return result;
}
