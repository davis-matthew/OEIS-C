/*
 * A161178  Sum of the double factorials of the digits of n.
 * https://oeis.org/A161178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum_{digits d of n} d!! (double factorial of each digit)
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed double factorials for digits 0-9 */
    static const seq_t dfact[10] = {1, 1, 2, 3, 8, 15, 48, 105, 384, 945};

    if (n == 0) {
        return dfact[0];            /* 0!! = 1 */
    }

    seq_t sum = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        int d = tmp % 10;          /* last digit */
        sum += dfact[d];
        tmp /= 10;                 /* remove last digit */
    }
    return sum;
}
