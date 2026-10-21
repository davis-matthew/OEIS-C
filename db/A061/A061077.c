/*
 * A061077  a(n) is the sum of the products of the digits of the first n odd
 *          numbers.
 * https://oeis.org/A061077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=1..n} product of the digits of (2*k-1)
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sum of the digit products of the first n odd numbers. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t odd = (seq_t)2 * k - 1;      /* the k-th odd number   */
        seq_t prod = 1;
        seq_t tmp  = odd;
        while (tmp > 0) {
            prod *= tmp % 10;
            tmp   /= 10;
        }
        sum += prod;
    }
    return sum;
}
