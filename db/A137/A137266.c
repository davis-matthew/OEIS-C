/*
 * A137266  a(n) = number of positive integers k where k divides (n -
 *          floor(n/k)).
 * https://oeis.org/A137266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k > 0 | k divides (n - floor(n/k)) }|
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 */

    idx_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t q = n / k;              /* floor(n/k) */
        idx_t diff = n - q;           /* n - floor(n/k) */
        if (diff % k == 0)
            ++count;
    }
    return (seq_t)count;
}
