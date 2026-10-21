/*
 * A055067  Product of numbers < n which do not divide n (or 1 if no such
 *          numbers exist).
 * https://oeis.org/A055067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product of all integers m with 1 ≤ m < n that do not
 *            divide n (a(1) = 1, a(2) = 1). Indices start at 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases: for n = 1 or 2 there are no eligible factors. */
    if (n <= 2) {
        return (seq_t)1;
    }

    seq_t prod = (seq_t)1;
    /* Multiply every integer k from 2 up to n-1 that is not a divisor of n. */
    for (idx_t k = 2; k < n; ++k) {
        if (n % k != 0) {
            prod *= (seq_t)k;
        }
    }
    return prod;
}
