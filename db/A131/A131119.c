/*
 * A131119  a(n) = (-1)^n * Sum_{i=1..floor(n/2)} i * floor(n/(n-i)).
 * https://oeis.org/A131119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * Sum_{i=1..floor(n/2)} i * floor(n/(n-i))
 * Range:     i8 n <= 29, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)          /* a(0) = a(1) = 0 */
        return 0;

    idx_t half = n / 2; /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= half; ++i) {
        idx_t denom = n - i;          /* n-i > 0   */
        idx_t k     = n / denom;      /* floor(n/(n-i)) */
        sum += (seq_t)i * (seq_t)k;   /* accumulate term */
    }

    return (n & 1) ? -sum : sum;      /* multiply by (-1)^n */
}
