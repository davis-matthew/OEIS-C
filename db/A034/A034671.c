/*
 * A034671  Sum of n-th powers of divisors of 72.
 * https://oeis.org/A034671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|72} d^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t divs[12] = {1, 2, 3, 4, 6, 8, 9, 12, 18, 24, 36, 72};
    seq_t sum = 0;
    for (idx_t i = 0; i < 12; ++i) {
        seq_t base = divs[i];
        seq_t pow = 1;
        idx_t e = n;
        while (e) {
            if (e & 1) pow *= base;
            base *= base;
            e >>= 1;
        }
        sum += pow;
    }
    return sum;
}
