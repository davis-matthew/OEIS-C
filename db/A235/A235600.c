/*
 * A235600  a(n) = n/d(n) if d(n) divides n, otherwise a(n) = n, where d(n) is
 *          the sum of the digits of n (A007953).
 * https://oeis.org/A235600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n/d(n) if d(n) divides n, otherwise a(n) = n, where d(n)
 *            is the sum of decimal digits of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* out of bounds, not expected */

    idx_t temp = n;
    idx_t sum = 0;
    while (temp > 0) {
        sum += temp % 10;
        temp /= 10;
    }

    if (sum == 0)
        return (seq_t)n;          /* should not happen for n >= 1 */

    if (n % sum == 0)
        return (seq_t)(n / sum);
    else
        return (seq_t)n;
}
