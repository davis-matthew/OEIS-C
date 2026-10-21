/*
 * A218738  a(n) = (35^n - 1)/34.
 * https://oeis.org/A218738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (35^n - 1)/34
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = sum_{k=0}^{n-1} 35^k */
/* a(0) = 0, a(1) = 1 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t sum   = 0;
    seq_t power = 1;
    for (idx_t i = 0; i < n; ++i) {
        sum += power;
        if (i + 1 < n) {
            power *= 35;
        }
    }
    return sum;
}
