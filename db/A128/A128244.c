/*
 * A128244  Let s be the sum of the digits of n; a(n) is the product of the
 *          digits of s.
 * https://oeis.org/A128244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = product of digits of sum of digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;
    seq_t sum = 0;
    while (t > 0) {
        sum += t % 10;
        t /= 10;
    }

    seq_t prod = 1;
    /* For n >= 1, sum is never zero, but handle it just in case */
    if (sum == 0) {
        return 0;
    }

    while (sum > 0) {
        prod *= sum % 10;
        sum /= 10;
    }

    return prod;
}
