/*
 * A061076  a(n) is the sum of the products of the digits of all the numbers
 *          from 1 to n.
 * https://oeis.org/A061076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = sum_{k=1..n} (product of the decimal digits of k)
 * Range:     u8 n <= 36, u16 n <= 867, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t prod = 1;
        idx_t x = i;
        while (x > 0) {
            int d = x % 10;          /* extract a decimal digit */
            if (d == 0) {            /* any zero digit makes the product zero */
                prod = 0;
                break;
            }
            prod *= (seq_t)d;        /* multiply the digit */
            x /= 10;
        }
        sum += prod;                 /* add to running total */
    }
    return sum;
}
