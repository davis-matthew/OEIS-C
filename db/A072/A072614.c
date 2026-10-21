/*
 * A072614  a(n) = Sum_{k = 1..n} (-1)^(n reduced mod k).
 * https://oeis.org/A072614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n mod k)
 * Range:     i8 n <= 379, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;          /* n mod k */
        if ((r & 1) == 0) {
            sum += 1;             /* remainder even → +1 */
        } else {
            sum -= 1;             /* remainder odd  → -1 */
        }
    }
    return sum;
}
