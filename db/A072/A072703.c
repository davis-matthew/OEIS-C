/*
 * A072703  Indices of Fibonacci numbers whose last digit is 5.
 * https://oeis.org/A072703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5 + 15*((n-1)/2) if n is odd, otherwise * a(n) = 10 +
 *            15*((n-2)/2)
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset starts at 1 */
    if ((n & 1) != 0) {              /* odd index */
        seq_t k = (seq_t)((n - 1) / 2);
        return 5 + 15 * k;
    }
    /* even index */
    seq_t k = (seq_t)((n - 2) / 2);
    return 10 + 15 * k;
}
